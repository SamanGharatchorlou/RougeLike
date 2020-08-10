#include "pch.h"
#include "Actors/Enemies/EnemyManager.h"

#include "Game/GameData.h"
#include "Map/Environment.h"
#include "Game/Camera.h"

#include "Enemy.h"
#include "Actors/Enemies/EnemyStates/EnemyAttack.h"



EnemyManager::EnemyManager(GameData* gameData) : mCollisions(gameData->collisionManager), mBuilder(gameData->textureManager) { }


EnemyManager::~EnemyManager()
{
	mActiveEnemies.clear();
}


void EnemyManager::clear()
{
	clearAllEnemies();
	mAIController.clearMap();
}


void EnemyManager::load()
{
	mAIController.loadAIPathMap(mEnvironment->primaryMap());
	mBuilder.loadSpawnPool();
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
	int a = 1;

	for (std::vector<Enemy*>::iterator iter = mActiveEnemies.begin(); iter != mActiveEnemies.end(); iter++)
	{
		Enemy* enemy = *iter;

		// Update enemies
		enemy->slowUpdate(dt);

		// Handle enemy messages
		while (enemy->events().hasEvent())
			mEvents.push(enemy->events().pop());
	}


	clearDead();

	mAIController.updatePaths(mActiveEnemies);
	mCollisions.updateAttackingColliders(attackingColliders());
}


void EnemyManager::spawn(const XMLNode spawnNode, const Map* map)
{
	std::vector<SpawnData> spawnData = mSpawner.getspawnList(spawnNode, map);

	for (int i = 0; i < spawnData.size(); i++)
	{
		spawnEnemy(spawnData[i]);
	}
}


void EnemyManager::render()
{
#if DRAW_AI_PATH_COSTMAP
	mAIController.drawCostMap();
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
	if (mActiveEnemies.size() > 0)
	{
		std::vector<Enemy*>::iterator iter = mActiveEnemies.begin();

		while (mActiveEnemies.size() > 0)
		{
			clearAndRemove(iter);
		}
	}

	ASSERT(Warning, mCollisions.isEmpty(), "Enemy colliders are left in the collision trackers when they shouldn't be!\n");
}



std::vector<Actor*> EnemyManager::getActiveEnemies() const
{ 
	std::vector<Actor*> actors;
	actors.reserve(mActiveEnemies.size());

	for (int i = 0; i < mActiveEnemies.size(); i++)
	{
		actors.push_back(mActiveEnemies[i]);
	}

	return actors;
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

void EnemyManager::spawnEnemy(const SpawnData spawnData)
{
#if LIMIT_ENEMY_SPAWNS
	if (mActiveEnemies.size() >= MAX_SPAWN_COUNT)
		return;
#endif

	Enemy* enemy = mBuilder.buildEnemy(spawnData, mEnvironment, mAIController.pathMap());
	mCollisions.add(enemy->collider());
	mActiveEnemies.push_back(enemy);
}


void EnemyManager::clearDead()
{
	for (std::vector<Enemy*>::iterator iter = mActiveEnemies.begin(); iter != mActiveEnemies.end(); iter++)
	{
		if ((*iter)->state() == EnemyState::Exit)
		{
			clearAndRemove(iter);

			if (iter == mActiveEnemies.end())
				break;
		}
	}
}

void EnemyManager::clearAndRemove(std::vector<Enemy*>::iterator& iter)
{
	Enemy* enemy = *iter;
	enemy->clear();

	mCollisions.remove(enemy->collider());
	mBuilder.returnEnemy(enemy);

	iter = mActiveEnemies.erase(iter);
}