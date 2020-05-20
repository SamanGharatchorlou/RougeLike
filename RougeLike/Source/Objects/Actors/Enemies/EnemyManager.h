#pragma once

#include "Events/Observer.h"
#include "Events/Dispatcher.h"

#include "Collisions/CollisionTracker.h"
#include "EnemyEnums.h"

#include "EnemyStates/EnemyState.h"
#include "Spawning/EnemySpawner.h"

#include "AI/AIPathMap.h"


struct GameData;
class Enemy;


class EnemyManager : public Dispatcher
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

	void clear();

	void init() { };
	void slowUpdate(float dt);
	void fastUpdate(float dt);
	void render() const;

	void handleEvent(EventData& data);

	void generatePathMap();
	void updateEnemyPaths();

	void clearAllEnemies();

	void addEnemiesToPool(EnemyType type, unsigned int count);

	void spawnLevel();
	void spawn(EnemyType type, EnemyState::Type state, VectorF position);

	void setTarget(RectF* rect) { mTarget = rect; }

	Enemy* getEnemy(unsigned int index) const;
	std::vector<Enemy*> getActiveEnemies() const { return mActiveEnemies; }

	unsigned int size() const { return mActiveEnemies.size(); }


private:

	void clearAndRemove(std::vector<Enemy*>::iterator& iter);

	void clearOccupiedTileInfo();
	void updateOccupiedTiles();

	void handleEnemyEvent(EventPacket eventPacket);

	Collider* getAttackingCollider(Enemy* enemy) const;

private:
	GameData* mGameData;

	AIPathMap mPathMap;

	std::vector<EnemyObject> mEnemyPool;
	std::vector<Enemy*> mActiveEnemies;

	RectF* mTarget;

	EnemySpawner mSpawner;

// TMEP
	int spawnCount = 0;
};