#include "pch.h"
#include "Characters/Enemies/EnemyManager.h"

#include "Game/GameData.h"
#include "Game/Camera.h"

#include "Items/Spawner.h"

#include "Characters/Enemies/Imp.h"


EnemyManager::EnemyManager(GameData* gameData) : mGameData(gameData) { }


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


void EnemyManager::addEnemiesToPool(EnemyType type, unsigned int count)
{
	// TODO: how to do a cheaper copy, no need to open and read files everytime... 
	for (unsigned int i = 0; i < count; i++)
	{
		EnemyObject enemyObject;
		enemyObject.second = ObjectStatus::Available;

		switch (type)
		{
		case EnemyType::None:
			break;
		case EnemyType::Imp:
			enemyObject.first = new Imp(mGameData);
			break;
		default:
			DebugPrint(Warning, "Invalid enemy type %d\n", type);
			break;
		}

		mEnemyPool.push_back(enemyObject);
	}

	DebugPrint(Log, "%d enemies of type %d added to the enemy pool, pool is now size %d\n", count, type, mEnemyPool.size());
}


void EnemyManager::spawn(EnemyType type, unsigned int xPositionPercentage)
{
	// Find available enemy to spawn
	for (unsigned int i = 0; i < mEnemyPool.size(); i++)
	{
		if (mEnemyPool[i].first->type() == type &&
			mEnemyPool[i].second == ObjectStatus::Available)
		{
			Enemy* enemy = mEnemyPool[i].first;

			// Add new active enemy to list and spawn around index x
			Spawner enemySpawner;
			VectorF position = enemySpawner.findSpawnPoint(mGameData->map, xPositionPercentage);
			enemy->spawn(position);

			mActiveEnemies.push_back(enemy);

			mEnemyPool[i].second = ObjectStatus::Active;
			enemy->setActive(true);

			enemy->setTarget(mTarget);

			// subscribe to player weapon collisions
			mCollisionManager.addDefender(enemy->getCollider());

			return;
		}
	}

	DebugPrint(Warning, "No available enemies of type %d in enemy pool, consider adding more\n", type);
}


void EnemyManager::fastUpdate(float dt)
{
	if (mActiveEnemies.size() > 0)
	{
		for (auto enemy : mActiveEnemies)
		{
			enemy->fastUpdate(dt);
		}

		// Player attacking enemies
		if (mCollisionManager.mAttackers.size() > 0)
			printf("");
		mCollisionManager.checkCollisions();
	}
}


void EnemyManager::slowUpdate(float dt)
{
	std::vector<Enemy*>::iterator iter;
	for (iter = mActiveEnemies.begin(); iter != mActiveEnemies.end(); iter++)
	{
		Enemy* enemy = *iter;

		// handle all enemy messages
		while (enemy->hasEvent())
		{
			EventPacket ep = enemy->popEvent();

			notify(ep.event, *ep.data);

			ep.free();
		}

		enemy->slowUpdate(dt);

		// Automatically clear out dead enemies
		// This needs to change to something else so the death anim can play
		if (!enemy->isActive())
		{
			deactivate(iter);

			if (iter == mActiveEnemies.end())
				break;
		}
	}

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


Enemy* EnemyManager::getEnemy(unsigned int index) const 
{
	ASSERT(Warning, index <= mActiveEnemies.size(), "Enemy index out of bounds\n");
	return mActiveEnemies[index];
}


void EnemyManager::render() const
{
	for (Enemy* enemy : mActiveEnemies)
	{
		if (Camera::Get()->inView(enemy->getRect()))
		{
			enemy->render();
		}
	}
}


// --- Collisions --- //

// Enemies are attacking with these colliders
std::vector<Collider*> EnemyManager::getAttackingColliders() const
{
	std::vector<Collider*> colliders;

	for (Enemy* enemy : mActiveEnemies)
	{
		if (enemy->getState() == EnemyState::Attack)
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
	// Find this enemy in the pool and deactivate inactive to be cleaned
	for (std::vector<EnemyObject>::iterator poolIter = mEnemyPool.begin(); poolIter != mEnemyPool.end(); poolIter++)
	{
		if (*iter == poolIter->first)
			poolIter->second = ObjectStatus::Inactive;
	}

	iter = mActiveEnemies.erase(iter);
}

