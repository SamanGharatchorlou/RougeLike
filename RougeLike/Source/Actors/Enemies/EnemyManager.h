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

// TEMP
#include "EnemyPool.h"


struct GameData;

class Environment;
class Enemy;
class Collider;
class TextureManager;
class EffectPool;

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
	void generateAIPathMap();
	void updateAIPathCostMap();
	void requestEnemyPathUpdates() { pathUpdateRequests++; }

	// Event handling
	EventPacket popEvent() { return mEvents.pop(); }
	void pushEvent(EventPacket event) { mEvents.push(event); }
	bool hasEvent() const { return mEvents.hasEvent(); }

	// Spawning
	void spawn(EnemyType type, EnemyState::Type state, VectorF position, TextureManager* textureManager, EffectPool* effectPool);

	std::vector<Enemy*> getActiveEnemies() const { return mActiveEnemies; }
	std::vector<Collider*> attackingColliders() const;

	unsigned int size() const { return mActiveEnemies.size(); }


private:
	void clearAndRemove(std::vector<Enemy*>::iterator& iter);

	void updateEnemyPaths();


private:
	Environment* mEnvironment;

	EnemyCollisions mCollisions;

	EnemyPool mPool;
	std::vector<Enemy*> mActiveEnemies;

	AIPathMap mPathMap;

	EnemySpawner mSpawner;

	LocalDispatcher mEvents;

	Timer<float> updateTimer;

	int pathUpdateRequests;
	int pathUpdateStaggerCounter;
};