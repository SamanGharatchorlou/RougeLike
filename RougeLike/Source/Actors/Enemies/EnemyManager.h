#pragma once

#include "Events/LocalDispatcher.h"

#include "Collisions/EnemyCollisions.h"
#include "Spawning/EnemySpawner.h"
#include "Actors/Enemies/Spawning/EnemyBuilder.h"

#include "AIController.h"

#if _DEBUG
#include "Debug/DebugDraw.h"
#endif


struct GameData;
class Environment;
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
	void spawn(const XMLParser& parser, const Map* map);


	std::vector<Enemy*> getActiveEnemies() const { return mActiveEnemies; }
	std::vector<Collider*> attackingColliders() const;

	unsigned int size() const { return mActiveEnemies.size(); }


private:
	void clearDead();
	void spawnEnemy(const SpawnData spawnData);
	void clearAndRemove(std::vector<Enemy*>::iterator& iter);


private:
	Environment* mEnvironment;

	AIController mAIController;

	EnemyCollisions mCollisions;	
	EnemySpawner mSpawner;
	EnemyBuilder mBuilder;

	std::vector<Enemy*> mActiveEnemies;

	LocalDispatcher mEvents;
};