#include "pch.h"
#include "Actors/Enemies/EnemyManager.h"

#include "Map/Map.h"
#include "Game/Environment.h"
#include "Game/Camera/Camera.h"

#include "Enemy.h"
#include "Actors/Enemies/EnemyStates/EnemyAttack.h"


EnemyManager::EnemyManager() : mEnvironment(nullptr) { }


EnemyManager::~EnemyManager()
{
	clear();
	mEnvironment = nullptr;
	ASSERT(Warning, mActiveEnemies.size() == 0, "All enemies should have been put back into the pool by now, there are %d left\n", mActiveEnemies.size());
}


void EnemyManager::clear()
{
	wipeEnemies(mActiveEnemies);
	mActiveEnemies.clear();
	mEvents.clear();
	mPathing.clear();
	mSpawning.clear();
}


void EnemyManager::loadPools()
{
	mSpawning.loadSpawnPool();
}

void EnemyManager::openNewMapLevel()
{
	Map* map = mEnvironment->lastMap(MapType::Dungeon);
	mPathing.addMap(map);

	const AIPathMap* aiPathMap = mPathing.pathMap(map);
	mSpawning.initSpawningEnemies(aiPathMap);
}


void EnemyManager::closeLastMapLevel()
{
	AIPathMap* pathMap = mPathing.popMap();

	std::vector<Enemy*>::iterator iter;
	for (iter = mActiveEnemies.begin(); iter != mActiveEnemies.end(); iter++)
	{
		if ((*iter)->pathing()->map() != pathMap)
		{
			break;
		}

		wipeEnemy(*iter);
	}

	mActiveEnemies = std::vector<Enemy*>(iter, mActiveEnemies.end());
}


void EnemyManager::init(Environment* environment)
{
	mEnvironment = environment;
	mSpawning.init();
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
		while (enemy->events().hasEvent())
			mEvents.push(enemy->events().pop());
	}

	clearDead();
	mPathing.updatePaths(mActiveEnemies, dt);
	mSpawning.spawnUnspawnedEnemies(this);
}


void EnemyManager::render()
{
#if DRAW_AI_PATH_COSTMAP
	mPathing.drawCostMap();
#endif

	for (Enemy* enemy : mActiveEnemies)
	{
		if (Camera::Get()->inView(enemy->rect()))
		{
			enemy->render();
		}
	}
}


void EnemyManager::resetColliders()
{
	for (int i = 0; i < mActiveEnemies.size(); i++)
	{
		mActiveEnemies[i]->resetColliders();
	}
}


void EnemyManager::clearAllEnemies()
{
	wipeEnemies(mActiveEnemies);
	mActiveEnemies.clear();
}



std::vector<Actor*> EnemyManager::getActiveEnemyActors() const
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

void EnemyManager::spawnEnemy(Enemy* enemy)
{
	enemy->set(mEnvironment);
#if !IGNORED_BY_ENEMIES
	enemy->initAbilities(mEnvironment->actors()->playerActorList());
	enemy->setTarget(mEnvironment->actors()->player()->get());
#endif

	mActiveEnemies.push_back(enemy);
}


// Can remove one per frame, no need to rush here...
void EnemyManager::clearDead()
{
	for (std::vector<Enemy*>::iterator iter = mActiveEnemies.begin(); iter != mActiveEnemies.end(); iter++)
	{
		if ((*iter)->state() == EnemyState::Exit)
		{
			clearAndRemove(iter);
			return;
		}
	}
}


void EnemyManager::wipeEnemies(std::vector<Enemy*>& enemies)
{
	for (int i = 0; i < enemies.size(); i++)
	{
		enemies[i]->clear();
		mSpawning.returnEnemy(enemies[i]);
		enemies[i] = nullptr;
	}
}


void EnemyManager::wipeEnemy(Enemy*& enemy)
{
	enemy->clear();
	mSpawning.returnEnemy(enemy);
	enemy = nullptr;
}

void EnemyManager::clearAndRemove(std::vector<Enemy*>::iterator& iter)
{
	(*iter)->clear();
	mSpawning.returnEnemy(*iter);
	iter = mActiveEnemies.erase(iter);
}