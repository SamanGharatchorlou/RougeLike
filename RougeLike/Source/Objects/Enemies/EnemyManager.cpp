#include "pch.h"
#include "Objects/Enemies/EnemyManager.h"

#include "Game/GameData.h"
#include "Collisions/CollisionManager.h"
#include "Map/Environment.h"
#include "Game/Camera.h"
#include "Objects/Player/PlayerManager.h"

#include "Objects/Enemies/Imp.h"

// State specific updates
#include "Objects/Enemies/EnemyStates/EnemyRun.h"
#include "Objects/Enemies/EnemyStates/EnemyAttack.h"

#include "Utilities/Shapes/Square.h"


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


void EnemyManager::clear()
{
	clearAllEnemies();
	mTarget = nullptr;
	mPathMap.clear();
}


void EnemyManager::init()
{
	addEnemiesToPool(EnemyType::Imp, 50);
}


void EnemyManager::fastUpdate(float dt)
{
	if (mActiveEnemies.size() > 0)
	{
		for (auto enemy : mActiveEnemies)
		{
			enemy->fastUpdate(dt);
		}
	}
}


void EnemyManager::slowUpdate(float dt)
{
	std::vector<Collider*> attackingColliders;

	for (std::vector<Enemy*>::iterator iter = mActiveEnemies.begin(); iter != mActiveEnemies.end(); iter++)
	{
		Enemy* enemy = *iter;

		// Update enemies
		enemy->slowUpdate(dt);

		// Handle enemy messages
		while (enemy->hasEvent())
			handleEnemyEvent(enemy);

		// Clear out dead enemies
		if (enemy->state() == EnemyState::None)
		{
			clearAndRemove(iter);

			if (iter == mActiveEnemies.end())
				break;
		}

		Collider* collider = getAttackingCollider(enemy);
		if (collider)
			attackingColliders.push_back(collider);
	}

	mGameData->collisionManager->removeAllAttackers(CollisionManager::Enemy_Hit_Player);
	mGameData->collisionManager->addAttackers(CollisionManager::Enemy_Hit_Player, attackingColliders);

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


void EnemyManager::generatePathMap()
{
	mPathMap.build(mGameData->environment->primaryMap(), 2, 2);
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
			Imp* imp = new Imp(mGameData);
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

			// TODO: add some kind of reset attribute thing here?
			//enemy->propertyBag().pHealth.get().setFullHp();
			enemy->spawn(state, position);
			enemy->setMap(&mPathMap);
			enemy->setAttackTarget(mTarget);

			std::vector<Collider*> defendingCollider;
			defendingCollider.push_back(enemy->collider());

			mGameData->collisionManager->addDefenders(CollisionManager::PlayerWeapon_Hit_Enemy, defendingCollider);

			mActiveEnemies.push_back(enemy);
			return;
		}
	}

	DebugPrint(Warning, "No available enemies of type %d in enemy pool of size %d, consider adding more\n", type, mEnemyPool.size());
}


void EnemyManager::spawnLevel()
{
	// Update ai path map
	generatePathMap();
	mSpawner.spawnLevel(mGameData->environment->primaryMap(), mGameData->environment->mapLevel());
}


void EnemyManager::handleEvent(const Event event, EventData& data)
{
	if (event == Event::UpdateAIPathMap)
	{
		updateEnemyPaths();
	}
}


// If player moves tile or an enemy moves a tile update this bad boi
void EnemyManager::updateEnemyPaths()
{
	clearOccupiedTileInfo();
	updateOccupiedTiles();

	for (int i = 0; i < mActiveEnemies.size(); i++)
	{
		if (mActiveEnemies[i]->state() == EnemyState::Run)
		{
			EnemyRun& runState = static_cast<EnemyRun&>(mActiveEnemies[i]->getStateMachine()->getActiveState());

			// No need to update anything if in attack range
			if (!runState.inAttackRange())
			{
				runState.updatePath();

 				Index nextTileIndex = runState.nextTileIndex();

				// Stop enemies walking over the same path on top of each other
				if (!nextTileIndex.isNegative())
					mPathMap.addToBeOccupiedTile(nextTileIndex);
			}
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


void EnemyManager::clearAllEnemies()
{
	for (Enemy* enemy : mActiveEnemies)
	{
		enemy->clear();
		mGameData->collisionManager->removeDefender(CollisionManager::PlayerWeapon_Hit_Enemy, enemy->collider());
	}

	for (EnemyObject& enemy : mEnemyPool)
	{
		 enemy.second = ObjectStatus::Available;
	}

	mActiveEnemies.clear();
}


// --- Collisions --- //

// Enemies are attacking with these colliders
Collider* EnemyManager::getAttackingCollider(Enemy* enemy) const
{
	if (enemy->state() == EnemyState::Attack)
	{
		const EnemyAttack* attackState = static_cast<const EnemyAttack*>(&(enemy->getStateMachine()->getActiveState()));

		if (!attackState->didConnectWithTarget())
		{
			return enemy->collider();
		}
	}

	return nullptr;
}



// --- Private Functions --- //

void EnemyManager::clearAndRemove(std::vector<Enemy*>::iterator& iter)
{
	(*iter)->clear();

	// Find this enemy in the pool
	for (std::vector<EnemyObject>::iterator poolIter = mEnemyPool.begin(); poolIter != mEnemyPool.end(); poolIter++)
	{
		if (*iter == poolIter->first)
			poolIter->second = ObjectStatus::Available;
	}

	iter = mActiveEnemies.erase(iter);
}



// Prevent enemies being on top of each other
void EnemyManager::updateOccupiedTiles()
{
	for (int i = 0; i < mActiveEnemies.size(); i++)
	{
		VectorF position = mActiveEnemies[i]->rect().Center();
		mPathMap.addOccupiedTile(position);
	}
}


void EnemyManager::clearOccupiedTileInfo()
{
	mPathMap.clearOccupiedTiles();
	mPathMap.clearToBeOccupiedTiles();
}


void EnemyManager::handleEnemyEvent(Enemy* enemy)
{
	EventPacket ep = enemy->popEvent();

	notify(ep.event, *ep.data);

	switch (ep.event)
	{
	case Event::EnemyDead:
		mGameData->collisionManager->removeDefender(CollisionManager::PlayerWeapon_Hit_Enemy, enemy->collider());
		break;
	}

	ep.free();
}