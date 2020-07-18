#pragma once

#include "Events/LocalDispatcher.h"

#include "Collisions/EnemyCollisions.h"

#include "Types/EnemyTypes.h"

#include "EnemyStates/EnemyState.h"
#include "Spawning/EnemySpawner.h"

#include "AI/AIPathMap.h"

#if _DEBUG
#include "Debug/DebugDraw.h"
#endif


struct GameData;

struct Environment;
class Enemy;
class Collider;

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
	void init(Environment* environment);
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

	std::vector<Enemy*> getActiveEnemies() const { return mActiveEnemies; }
	std::vector<Collider*> attackingColliders() const;

	unsigned int size() const { return mActiveEnemies.size(); }


private:
	void clearAndRemove(std::vector<Enemy*>::iterator& iter);

	void updateEnemyPaths();


private:
	GameData* mGameData;
	Environment* mEnvironment;

	EnemyCollisions mCollisions;

	// move this functionality into a class?
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