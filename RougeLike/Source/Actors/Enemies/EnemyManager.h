#pragma once

#include "Events/LocalDispatcher.h"

#include "Collisions/CollisionTracker.h"
#include "Types/EnemyTypes.h"

#include "EnemyStates/EnemyState.h"
#include "Spawning/EnemySpawner.h"

#include "AI/AIPathMap.h"

#if _DEBUG
#include "Debug/DebugDraw.h"
#endif


struct GameData;
class Enemy;


class EnemyManager
{
public:
	enum ObjectStatus
	{
		None,
		Uninitialised,
		Available,
		Active,
		Inactive,
	};

	using EnemyObject = std::pair<Enemy*, ObjectStatus>;

public:
	EnemyManager(GameData* gameData);
	~EnemyManager();

	// Core
	void load();
	void init() { };
	void slowUpdate(float dt);
	void fastUpdate(float dt);
	void render();

	void clear();
	void clearAllEnemies();

	// AI pathing
	void generatePathMap();
	void updateAIPathCostMap();
	void requestEnemyPathUpdates() { pathUpdateRequests++; }

	// Event handling
	EventPacket popEvent() { return mEvents.pop(); }
	void pushEvent(EventPacket event) { mEvents.push(event); }
	bool hasEvent() const { return mEvents.hasEvent(); }

	// Spawning
	void addEnemiesToPool(EnemyType type, unsigned int count);
	void spawnLevel();
	void spawn(EnemyType type, EnemyState::Type state, VectorF position);

	// General
	Enemy* getEnemy(unsigned int index) const;
	std::vector<Enemy*> getActiveEnemies() const { return mActiveEnemies; }

	unsigned int size() const { return mActiveEnemies.size(); }


private:
	void clearAndRemove(std::vector<Enemy*>::iterator& iter);

	void updateEnemyPaths();

	Collider* getAttackingCollider(Enemy* enemy) const;

private:
	GameData* mGameData;

	std::vector<EnemyObject> mEnemyPool;
	std::vector<Enemy*> mActiveEnemies;

	AIPathMap mPathMap;

	EnemySpawner mSpawner;

	LocalDispatcher mEvents;

	Timer<float> updateTimer;

	int pathUpdateRequests;
	int pathUpdateStaggerCounter;

#if LIMIT_ENEMY_SPAWNS
	int spawnCount = 0;
#endif
};