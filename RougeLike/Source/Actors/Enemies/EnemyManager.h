#pragma once

#include "Events/LocalDispatcher.h"

#include "AI/Pathing/AIPathingController.h"
#include "AI/Spawning/AISpawnController.h"


class Environment;
class Actor;
class Enemy;
class Collider;

#include "Debug/PerformanceProfiler.h"


class EnemyManager
{
public:
	EnemyManager();
	~EnemyManager();

	// Core
	void loadPools();
	void init(Environment* environment);
	void slowUpdate(float dt);
	void fastUpdate(float dt);
	void render();

	void clear();
	void clearAllEnemies();
	void resetColliders();

	// Levels
	void openNewMapLevel();
	void closeLastMapLevel();

	// Event handling
	LocalDispatcher& events() { return mEvents; }

	std::vector<Enemy*>& getActiveEnemies() { return mActiveEnemies; }
	std::vector<Actor*> getActiveEnemyActors() const;
	std::vector<Collider*> attackingColliders() const;

	unsigned int size() const { return mActiveEnemies.size(); }


	void spawnEnemy(Enemy* enemy);
private:
	void clearDead();
	void wipeEnemies(std::vector<Enemy*>& enemies);
	void wipeEnemy(Enemy*& enemy);
	void clearAndRemove(std::vector<Enemy*>::iterator& iter);


private:
	Environment* mEnvironment;

	AIPathingController mPathing;
	AISpawnController mSpawning;

	LocalDispatcher mEvents;

	std::vector<Enemy*> mActiveEnemies;

	PerformanceProfiler profiler;
};