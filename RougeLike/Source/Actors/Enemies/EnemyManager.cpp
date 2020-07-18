#include "pch.h"
#include "Actors/Enemies/EnemyManager.h"

#include "Game/GameData.h"
#include "Collisions/CollisionManager.h"
#include "Map/Environment.h"
#include "Game/Camera.h"
#include "Actors/ActorManager.h"
#include "Actors/Player/Player.h"

#include "Types/Devil.h"

// State specific updates
#include "Actors/Enemies/EnemyStates/EnemyRun.h"
#include "Actors/Enemies/EnemyStates/EnemyAttack.h"

#if DRAW_AI_PATH_COSTMAP
#include "UI/Elements/UITextBox.h"
#endif


EnemyManager::EnemyManager(GameData* gameData) : 
	mGameData(gameData), mCollisions(gameData->collisionManager), pathUpdateRequests(0), pathUpdateStaggerCounter(0)
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
	mPathMap.clear();
}


void EnemyManager::init(Environment* environment)
{
	mEnvironment = environment;
}


void EnemyManager::fastUpdate(float dt)
{
	for (int i = 0; i < mActiveEnemies.size(); i++)
	{
		mActiveEnemies[i]->fastUpdate(dt);
	}
}


void EnemyManager::slowUpdate(float dt)
{
	for (std::vector<Enemy*>::iterator iter = mActiveEnemies.begin(); iter != mActiveEnemies.end(); iter++)
	{
		Enemy* enemy = *iter;

		// Update enemies
		enemy->slowUpdate(dt);

		// Handle enemy messages
		while (enemy->hasEvent())
			mEvents.push(enemy->popEvent());

		// Clear out dead enemies
		if (enemy->state() == EnemyState::Exit)
		{
			clearAndRemove(iter);

			if (iter == mActiveEnemies.end())
				break;
		}
	}

	if (pathUpdateStaggerCounter != 0 || pathUpdateRequests)
	{
		updateEnemyPaths();
	}

	mCollisions.updateAttackingColliders(attackingColliders());

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
	mPathMap.build(mEnvironment->primaryMap(), 4, 4);
}


void EnemyManager::addEnemiesToPool(EnemyType type, unsigned int count)
{
	for (unsigned int i = 0; i < count; i++)
	{
		EnemyObject enemyObject;
		enemyObject.second = ObjectStatus::Uninitialised;

		switch (type)
		{
		case EnemyType::None:
			enemyObject.second = ObjectStatus::None;
			break;
		case EnemyType::Devil:
		{
			Devil* devil= new Devil();
			devil->init("Devil", mGameData->environment, mGameData->textureManager);

			enemyObject.first = devil;
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
			enemy->Actor::init(mEnvironment);
			enemy->setMap(&mPathMap);
			enemy->spawn(state, position);
#if !IGNORED_BY_ENEMIES
			enemy->setTarget(mGameData->actors->player());
#endif

			mCollisions.add(enemy->collider());

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
	//mSpawner.spawnLevel(mGameData->environment->primaryMap(), mGameData->environment->mapLevel());


}


void EnemyManager::load()
{
	// Set ai path map
	generatePathMap();
	addEnemiesToPool(EnemyType::Devil, 50);
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


void EnemyManager::render()
{
#if DRAW_AI_PATH_COSTMAP
	// Replace all this with the debugRenderText function
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

			if (Camera::Get()->inView(rect) && mPathMap[index].hasCollisionType(MapTile::Floor))
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
		mCollisions.remove(enemy->collider());
		enemy->clear();
	}

	ASSERT(Warning, mCollisions.isEmpty(), "Enemy colliders are left in the collision trackers when they shouldn't be!\n");

	//mGameData->collisionManager->removeAllDefenders(CollisionManager::PlayerWeapon_Hit_Enemy);
	//mGameData->collisionManager->removeAllDefenders(CollisionManager::Player_Hit_Enemy);
	//mGameData->collisionManager->removeAllAttackers(CollisionManager::Enemy_Hit_Player);



	for (EnemyObject& enemy : mEnemyPool)
	{
		 enemy.second = ObjectStatus::Available;
	}

	mActiveEnemies.clear();
}


// --- Collisions --- //
std::vector<Collider*> EnemyManager::attackingColliders() const
{
	std::vector<Collider*> colliders;

	for (std::vector<Enemy*>::const_iterator iter = mActiveEnemies.begin(); iter != mActiveEnemies.end(); iter++)
	{
		Enemy* enemy = *iter;
		if (enemy->state() == EnemyState::Attack)
		{
			const EnemyAttack* attackState = static_cast<const EnemyAttack*>(&(enemy->getStateMachine()->getActiveState()));
			if (!attackState->didConnectWithTarget())
			{
				Collider* collider = enemy->collider();
				colliders.push_back(collider);
			}
		}
	}

	return colliders;
}



// --- Private Functions --- //

void EnemyManager::clearAndRemove(std::vector<Enemy*>::iterator& iter)
{
	mCollisions.remove((*iter)->collider());
	(*iter)->clear();

	// Find this enemy in the pool
	for (std::vector<EnemyObject>::iterator poolIter = mEnemyPool.begin(); poolIter != mEnemyPool.end(); poolIter++)
	{
		if (*iter == poolIter->first)
			poolIter->second = ObjectStatus::Available;
	}

	iter = mActiveEnemies.erase(iter);
}