#pragma once

#include "Events/Dispatcher.h"

#include "Collisions/CollisionTracker.h"
#include "EnemyEnums.h"
#include "EnemySpawner.h"

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

	void slowUpdate(float dt);
	void fastUpdate(float dt);

	void subscribe(std::vector<Collider*> colliders);

	void addEnemiesToPool(EnemyType type, int count);

	void spawn(EnemyType type, float xIndex);

	void setTarget(RectF* rect) { mTarget = rect; }

	Enemy* getEnemy(unsigned int index);

	std::vector<Collider*> getAttackingEnemyColliders();

	unsigned int size() { return mActiveEnemies.size(); }

	void render();

private:
	void deactivate(std::vector<Enemy*>::iterator& iter);

private:
	GameData* mGameData;

	std::vector<EnemyObject> mEnemyPool;
	std::vector<Enemy*> mActiveEnemies;

	CollisionTracker mCollisionManager;

	EnemySpawner mSpawner;

	RectF* mTarget;
};