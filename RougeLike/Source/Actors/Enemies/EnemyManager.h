#pragma once

#include "Events/LocalDispatcher.h"

#include "AI/Pathing/AIPathingController.h"
#include "AI/Spawning/AISpawnController.h"


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
	void loadPools();
	void init(Environment* environment);
	void slowUpdate(float dt);
	void fastUpdate(float dt);
	void render();

	void clear();
	void removeActiveEnemies(std::vector<Enemy*> enemies);
	void clearAllEnemies();

	// Levels
	void openNewMapLevel();
	void closeLastMapLevel();

	// AI pathing
	void updateAIPathCostMap() { mPathing.updateAIPathCostMap(mActiveEnemies); }
	void requestEnemyPathUpdates() { mPathing.addPathUpdateRequest(); }

	// Event handling
	LocalDispatcher& events() { return mEvents; }

	std::vector<Actor*> getActiveEnemies() const;
	std::vector<Collider*> attackingColliders() const;

	unsigned int size() const { return mActiveEnemies.size(); }


private:
	void spawnEnemies(const std::vector<Enemy*>& enemies);
	void addActiveEnemy(Enemy* enemy);

	void clearDead();
	void clearAndRemove(std::vector<Enemy*>::iterator& iter);
	void clearActiveEnemy(Enemy* enemy);


private:
	Environment* mEnvironment;

	AIPathingController mPathing;
	AISpawnController mSpawning;

	LocalDispatcher mEvents;

	std::vector<Enemy*> mActiveEnemies;
};