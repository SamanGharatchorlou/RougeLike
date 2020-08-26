#include "pch.h"
#include "Actors/Enemies/EnemyManager.h"

#include "Game/Environment.h"
#include "Game/Camera/Camera.h"

#include "Enemy.h"
#include "Actors/Enemies/EnemyStates/EnemyAttack.h"

// temp
#include "Map/Map.h"



EnemyManager::EnemyManager() : mEnvironment(nullptr) { }


EnemyManager::~EnemyManager()
{
	clear();
	mEnvironment = nullptr;
	ASSERT(Warning, mActiveEnemies.size() == 0, "All enemies should have been put back into the pool by now, there are %d left\n", mActiveEnemies.size());
}


void EnemyManager::clear()
{
	clearAllEnemies();
	mEvents.clear();

	mAIController.clear();
	mBuilder.clear();
	mCollisions.clear();
}


void EnemyManager::loadPools()
{
	int enemyPoolSize = 150;
	mBuilder.loadPools(enemyPoolSize);
}




void EnemyManager::init(Environment* environment, CollisionManager* collisions)
{
	mEnvironment = environment;
	mCollisions.init(collisions);
}


void EnemyManager::fastUpdate(float dt)
{
	for (int i = 0; i < mActiveEnemies.size(); i++)
	{
		mActiveEnemies[i]->fastUpdate(dt);
	}
}

void EnemyManager::addNewAIPathMap()
{
	Map* map = mEnvironment->lastMap(MapType::Dungeon);
	mAIController.addMap(map);

	// TODO: store these values? in the spawner, then change them as the levels move on
	BasicString path = FileManager::Get()->findFile(FileManager::Config_Map, "Environment");
	XMLParser parser(path);
	spawn(parser.rootChild("Enemies"), map);
}


void EnemyManager::popAIPathMap()
{
	AIPathMap* pathMap = mAIController.popMap();

	std::vector<Enemy*>::iterator lastInvalidEnemy;

	for (lastInvalidEnemy = mActiveEnemies.begin(); lastInvalidEnemy != mActiveEnemies.end(); lastInvalidEnemy++)
	{
		if ((*lastInvalidEnemy)->getAIPathing()->pathMap() != pathMap)
		{
			if(lastInvalidEnemy != mActiveEnemies.begin())
				lastInvalidEnemy - 1;

			break;
		}
	}

	std::vector<Enemy*> enemiesToRemove(mActiveEnemies.begin(), lastInvalidEnemy);

	std::vector<Enemy*>::iterator firstValidEnemy = lastInvalidEnemy != mActiveEnemies.end() ? lastInvalidEnemy + 1 : mActiveEnemies.end();
	mActiveEnemies = std::vector<Enemy*>(firstValidEnemy, mActiveEnemies.end());

	removeActiveEnemies(enemiesToRemove);
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

	mAIController.updatePaths(mActiveEnemies);
	mCollisions.updateAttackingColliders(attackingColliders());
}


void EnemyManager::spawn(const XMLNode levelSpawnNode, const Map* map)
{
	std::vector<SpawnData> spawnData = mSpawner.getspawnList(levelSpawnNode, map);
	spawnEnemies(spawnData, mAIController.pathMap(map));
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

	mActiveEnemies.clear();

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

void EnemyManager::spawnEnemies(const std::vector<SpawnData>& spawnData, AIPathMap* aiPathMap)
{
	std::vector<Enemy*> enemies = mBuilder.buildEnemies(spawnData, aiPathMap);

	for (int i = 0; i < enemies.size(); i++)
	{
#if LIMIT_ENEMY_SPAWNS
		if (mActiveEnemies.size() >= MAX_SPAWN_COUNT)
			mBuilder.returnEnemy(enemies[i]);
		else
#endif
			addActiveEnemy(enemies[i]);
	}
}


void EnemyManager::addActiveEnemy(Enemy* enemy)
{
	enemy->set(mEnvironment);
#if !IGNORED_BY_ENEMIES
	enemy->setTarget(mEnvironment->actors()->player()->get());
#endif


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


void EnemyManager::removeActiveEnemies(std::vector<Enemy*> enemies)
{
	for (int i = 0; i < enemies.size(); i++)
	{
		clearActiveEnemy(enemies[i]);
	}
}


void EnemyManager::clearActiveEnemy(Enemy* enemy)
{
	enemy->clear();

	mCollisions.remove(enemy->collider());
	mBuilder.returnEnemy(enemy);
	enemy = nullptr;
}