#include "pch.h"
#include "Objects/Actors/Enemies/EnemyManager.h"

#include "Game/GameData.h"
#include "Collisions/CollisionManager.h"
#include "Map/Environment.h"
#include "Game/Camera.h"
#include "Objects/Actors/Player/Player.h"

#include "Types/Imp.h"
#include "Types/Angel.h"

// State specific updates
#include "Objects/Actors/Enemies/EnemyStates/EnemyRun.h"
#include "Objects/Actors/Enemies/EnemyStates/EnemyAttack.h"

#include "Utilities/Shapes/Square.h"

#if DRAW_AI_PATH_COSTMAP
#include "UI/Elements/UITextBox.h"
#endif


EnemyManager::EnemyManager(GameData* gameData) : mGameData(gameData), mSpawner(this), pathUpdateRequests(0), pathUpdateStaggerCounter(0)
{
	updateTimer.start();
}

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
			mEvents.push(enemy->popEvent());

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

	if (pathUpdateStaggerCounter != 0 || pathUpdateRequests)
	{
		updateEnemyPaths();
	}

	if (updateTimer.getMilliseconds() > 250)
	{
		updateAIPathCostMap();
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
			imp->init(); // TODO: better way to feed this in?

			enemyObject.first = imp;
			enemyObject.second = ObjectStatus::Available;
			break;
		}
		case EnemyType::Angel:
		{
			Angel* angel = new Angel(mGameData);
			angel->init(); // TODO: better way to feed this in?

			enemyObject.first = angel;
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
#if LIMIT_ENEMY_SPAWNS
	if (spawnCount >= LIMIT_ENEMY_SPAWNS - 1)
		return;
#endif

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
#if LIMIT_ENEMY_SPAWNS
			spawnCount++;
#endif
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


// If player moves tile or an enemy moves a tile update this bad boi
void EnemyManager::updateEnemyPaths()
{
	mPathMap.costMap().setAllValues(1);
	updateAIPathCostMap();

	int loopCount = 0;

	for (; pathUpdateStaggerCounter < mActiveEnemies.size(); pathUpdateStaggerCounter++)
	{
		// Only allow a number of these (expensive) updates per frame, reduce stuttering
		if (loopCount++ >= 3)
			return;

		if (mActiveEnemies[pathUpdateStaggerCounter]->state() == EnemyState::Run)
		{
			EnemyRun& runState = static_cast<EnemyRun&>(mActiveEnemies[pathUpdateStaggerCounter]->getStateMachine()->getActiveState());

			// No need to update anything if in attack range
			if (!runState.inAttackRange())
			{
				runState.updatePath();
			}
		}
	}

	if (pathUpdateStaggerCounter == mActiveEnemies.size())
	{
		pathUpdateStaggerCounter = 0;
		pathUpdateRequests--;

		ASSERT(Warning, pathUpdateRequests >= 0, "Cannot have negative path update requests, count: %d\n", pathUpdateRequests);

		// No reason to have more than 1 request queued up after completing a request
		pathUpdateRequests = clamp(pathUpdateStaggerCounter, 0, 1);
	}
}


void EnemyManager::updateAIPathCostMap()
{
	Grid<int>& AICostMap = mPathMap.costMap();
	AICostMap.setAllValues(1);

	for (int i = 0; i < mActiveEnemies.size(); i++)
	{
		// Current tile
		Index index = mPathMap.index(mActiveEnemies[i]->position());
		AICostMap[index] += 10;

		// Immediate surrounding tiles
		Index surroundingIndexsLayer1[8]{
			Index(index + Index(-1,-1)),
			Index(index + Index(+0, -1)),
			Index(index + Index(+1,-1)),

			Index(index + Index(-1,0)),
			Index(index + Index(+1,0)),

			Index(index + Index(-1,+1)),
			Index(index + Index(+0, +1)),
			Index(index + Index(+1,+1))
		};

		for (int i = 0; i < 8; i++)
		{
			AICostMap[surroundingIndexsLayer1[i]] += 2;
		}
	}
}


Enemy* EnemyManager::getEnemy(unsigned int index) const 
{
	ASSERT(Warning, index <= mActiveEnemies.size(), "Enemy index out of bounds\n");
	return mActiveEnemies[index];
}


void EnemyManager::render()
{
#if DRAW_AI_PATH_COSTMAP
	UITextBox::Data textData;
	textData.aligment = "";
	textData.font = "";
	textData.ptSize = 16;
	textData.colour = SDL_Color{ 255, 0, 0 };
	textData.texture = nullptr;
	// to be set
	textData.rect = RectF(); 
	textData.text = "";

	UITextBox text(textData);

	char cost[3];

	for (int y = 0; y < mPathMap.yCount(); y++)
	{
		for (int x = 0; x < mPathMap.xCount(); x++)
		{
			Index index(x, y);
			RectF rect = mPathMap[index].rect();

			if (Camera::Get()->inView(rect))
			{
				rect = Camera::Get()->toCameraCoords(rect);

				_itoa_s(mPathMap.costMap()[index], cost, 10);

				text.setText(cost);
				text.setRect(rect);

				text.render();
			}
		}
	}
#endif


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
		mGameData->collisionManager->removeAllAttackers(CollisionManager::Enemy_Hit_Player);
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