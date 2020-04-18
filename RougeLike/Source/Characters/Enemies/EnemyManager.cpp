#include "pch.h"
#include "Characters/Enemies/EnemyManager.h"

#include "Game/GameData.h"
#include "Map/MapLevel.h"
#include "Game/Camera.h"
#include "Characters/Player/PlayerManager.h"

#include "Items/Spawner.h"

#include "Characters/Enemies/Imp.h"

// TEMP
#include "Characters/Enemies/EnemyStates/EnemyRun.h"

// TODO: remove enemies once out of play, i.e. player has not killed them and moved onto the next level
EnemyManager::EnemyManager(GameData* gameData) : mGameData(gameData), mSpawner(this) { }


EnemyManager::~EnemyManager()
{
	for (auto enemy : mEnemyPool)
	{
		delete enemy.first;
		enemy.first = nullptr;
		enemy.second = ObjectStatus::None;
	}

	mActiveEnemies.clear();
	mEnemyPool.clear();
}


void EnemyManager::generatePathMap()
{
	mPathMap.build(mGameData->level->primaryMap(), 2, 2);
}


void EnemyManager::addEnemiesToPool(EnemyType type, unsigned int count)
{
	// TODO: how to do a cheaper copy, no need to open and read files everytime... 
	for (unsigned int i = 0; i < count; i++)
	{
		EnemyObject enemyObject;
		enemyObject.second = ObjectStatus::Uninitialised;

		switch (type)
		{
		case EnemyType::None:
			enemyObject.second = ObjectStatus::None;
			break;

		case EnemyType::Imp:
		{
			Imp* imp = new Imp(mGameData, &mPathMap);
			imp->init();

			enemyObject.first = imp;
			enemyObject.second = ObjectStatus::Available;
			break;
		}
		default:
			DebugPrint(Warning, "Invalid enemy type %d\n", type);
			break;
		}

		mEnemyPool.push_back(enemyObject);
	}

	DebugPrint(Log, "%d enemies of type %d added to the enemy pool, pool is now size %d\n", count, type, mEnemyPool.size());
}


void EnemyManager::spawn(EnemyType type, EnemyState::Type state, VectorF position)
{
	// Find available enemy to spawn
	for (unsigned int i = 0; i < mEnemyPool.size(); i++)
	{
		if (mEnemyPool[i].first->type() == type &&
			mEnemyPool[i].second == ObjectStatus::Available)
		{
			Enemy* enemy = mEnemyPool[i].first;
			mEnemyPool[i].second = ObjectStatus::Active;

			enemy->propertyBag().pHealth.get().setFullHp();
			enemy->spawn(state, position);
			enemy->setTarget(mTarget);

			// subscribe to player weapon collisions
			mCollisionManager.addDefender(enemy->getCollider());

			mActiveEnemies.push_back(enemy);
			return;
		}
	}

	DebugPrint(Warning, "No available enemies of type %d in enemy pool, consider adding more\n", type);
}

void EnemyManager::spawnLevel()
{
	// TODO: can i just link this up to level and forget about updating it?
	mSpawner.spawnLevelPatrollers(mGameData->level->primaryMap());
}


void EnemyManager::fastUpdate(float dt)
{
	if (mActiveEnemies.size() > 0)
	{
		for (auto enemy : mActiveEnemies)
		{
			enemy->fastUpdate(dt);
		}

		mCollisionManager.checkCollisions();
	}
}


void EnemyManager::slowUpdate(float dt)
{
	for (std::vector<Enemy*>::iterator iter = mActiveEnemies.begin(); iter != mActiveEnemies.end(); iter++)
	{
		Enemy* enemy = *iter;

		// Handle enemy messages
		while (enemy->hasEvent())
			handleEvent(enemy);

		// Update enemies
		enemy->slowUpdate(dt);

		// Clear out dead enemies
		if (enemy->state() == EnemyState::None)
		{
			deactivate(iter);

			if (iter == mActiveEnemies.end())
				break;
		}
	}

	// Update weapon collider list
	clearAttackingColliders();
	addAttackingColliders(mGameData->playerManager->getWeaponColliders());

#if _DEBUG // Police the pool and active enemy lists
	int activeEnemies = 0;
	for (unsigned int i = 0; i < mEnemyPool.size(); i++)
	{
		if (mEnemyPool[i].second == ObjectStatus::Active)
			activeEnemies++;
	}

	ASSERT(Warning, activeEnemies == mActiveEnemies.size(),
		"There is a mismatch between the number of active enemies in the pool(%d) and the number active in the active list %d\n",
		activeEnemies, mActiveEnemies.size());
#endif
}


void EnemyManager::handleEvent(const Event event, EventData& data)
{
	if (event == Event::UpdateAIPathMap)
	{
		updateEnemyPaths();
	}
}


// Call from messages?
// if player moves tile or an enemy moves a tile update this bad boi
void EnemyManager::updateEnemyPaths()
{
	updateOccupiedTiles();
	mPathMap.clearToBeOccupiedTiles();

	for (int i = 0; i < mActiveEnemies.size(); i++)
	{
		if (mActiveEnemies[i]->state() == EnemyState::Run)
		{
			EnemyRun& runState = static_cast<EnemyRun&>(mActiveEnemies[i]->getStateMachine()->getActiveState());
			runState.updatePath();

			Index nextTileIndex = runState.nextTileIndex();
			mPathMap.addToBeOccupiedTile(nextTileIndex);
		}
	}
}


Enemy* EnemyManager::getEnemy(unsigned int index) const 
{
	ASSERT(Warning, index <= mActiveEnemies.size(), "Enemy index out of bounds\n");
	return mActiveEnemies[index];
}


void EnemyManager::render() const
{
	for (Enemy* enemy : mActiveEnemies)
	{
		if (Camera::Get()->inView(enemy->rect()))
		{
			enemy->render();
		}
	}
}


void EnemyManager::destroyAllEnemies()
{
	for (unsigned int i = 0; i < mActiveEnemies.size(); i++)
	{
		mActiveEnemies[i]->addState(EnemyState::None);
	}
}


// --- Collisions --- //

// Enemies are attacking with these colliders
std::vector<Collider*> EnemyManager::getAttackingColliders() const
{
	std::vector<Collider*> colliders;

	for (Enemy* enemy : mActiveEnemies)
	{
		if (enemy->state() == EnemyState::Attack)
		{
			DamageCollider* collider = enemy->getCollider();

			if (!collider->didHit())
			{
				colliders.push_back(collider);
			}
		}

	}

	return colliders;
}


// Enemies being attacked by these colliders
void EnemyManager::clearAttackingColliders()
{
	mCollisionManager.clearAttackers();
}


void EnemyManager::addAttackingColliders(std::vector<Collider*> colliders)
{
	mCollisionManager.addAttackers(colliders);
}


// --- Private Functions --- //
void EnemyManager::deactivate(std::vector<Enemy*>::iterator& iter)
{
	// Find this enemy in the pool, deactivate and clear from active enemies
	for (std::vector<EnemyObject>::iterator poolIter = mEnemyPool.begin(); poolIter != mEnemyPool.end(); poolIter++)
	{
		if (*iter == poolIter->first)
			poolIter->second = ObjectStatus::Inactive;
	}

	(*iter)->clear();
	iter = mActiveEnemies.erase(iter);
}


void EnemyManager::updateOccupiedTiles()
{
	// Update AI Path mapping first
	mPathMap.clearOccupiedTiles();

	for (int i = 0; i < mActiveEnemies.size(); i++)
	{
		VectorF position = mActiveEnemies[i]->rect().Center();
		mPathMap.addOccupiedTile(position);
	}
}


void EnemyManager::handleEvent(Enemy* enemy)
{
	EventPacket ep = enemy->popEvent();

	notify(ep.event, *ep.data);

	ep.free();
}