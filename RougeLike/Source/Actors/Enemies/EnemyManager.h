#pragma once

#include "Events/LocalDispatcher.h"

#include "Collisions/EnemyCollisions.h"
#include "Spawning/EnemySpawner.h"
#include "Actors/Enemies/Spawning/EnemyBuilder.h"

#include "AIController.h"


struct GameData;
class Environment;
class Actor;
class Enemy;
class Collider;

class EnemyManager
{
public:
	EnemyManager(GameData* gameData);
	~EnemyManager();

	// Core
	void load();
	void init(Environment* environment);
	void slowUpdate(float dt);
	void fastUpdate(float dt);
	void render();

	void clear();
	void clearAllEnemies();

	// AI pathing
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
	void spawnEnemies(const std::vector<SpawnData>& spawnData);

	void clearDead();
	void clearAndRemove(std::vector<Enemy*>::iterator& iter);


private:
	Environment* mEnvironment;

	AIController mAIController;

	EnemySpawner mSpawner;
	EnemyBuilder mBuilder;
	EnemyCollisions mCollisions;

	LocalDispatcher mEvents;

	std::vector<Enemy*> mActiveEnemies;
};