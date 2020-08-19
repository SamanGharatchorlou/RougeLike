#pragma once

#include "Events/LocalDispatcher.h"

#include "Collisions/EnemyCollisions.h"
#include "Spawning/EnemySpawner.h"
#include "Actors/Enemies/Spawning/EnemyBuilder.h"

#include "AIController.h"


class Environment;
class Actor;
class Enemy;
class Collider;


class EnemyManager
{
public:
	EnemyManager();
	~EnemyManager();

	// Core
	void load();
	void init(Environment* environment, CollisionManager* collisions);
	void slowUpdate(float dt);
	void fastUpdate(float dt);
	void render();

	void clear();
	void removeActiveEnemies(std::vector<Enemy*> enemies);
	void clearAllEnemies();

	// AI pathing
	void addNewAIPathMap();
	void popAIPathMap();
	void updateAIPathCostMap() { mAIController.updateAIPathCostMap(mActiveEnemies); }
	void requestEnemyPathUpdates() { mAIController.addPathUpdateRequest(); }

	// Event handling
	LocalDispatcher& events() { return mEvents; }

	// Spawning
	void spawn(const XMLNode levelSpawnNode, const Map* map);

	std::vector<Actor*> getActiveEnemies() const;
	std::vector<Collider*> attackingColliders() const;

	unsigned int size() const { return mActiveEnemies.size(); }


private:
	void addActiveEnemy(Enemy* enemy);
	void spawnEnemies(const std::vector<SpawnData>& spawnData, AIPathMap* aiPathMap);

	void clearDead();
	void clearAndRemove(std::vector<Enemy*>::iterator& iter);
	void clearActiveEnemy(Enemy* enemy);


private:
	Environment* mEnvironment;

	AIController mAIController;

	EnemySpawner mSpawner;
	EnemyBuilder mBuilder;
	EnemyCollisions mCollisions;

	LocalDispatcher mEvents;

	std::vector<Enemy*> mActiveEnemies;
};