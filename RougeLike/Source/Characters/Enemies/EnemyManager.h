#pragma once

#include "Collisions/ColliderManager.h"
#include "EnemyEnums.h"
#include "EnemySpawner.h"

struct GameData;
class Enemy;
class Weapon;

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

	void slowUpdate(float dt);
	void fastUpdate(float dt);

	void subscribePlayerWeaponCollisions(Weapon* weapon);

	void addEnemiesToPool(EnemyType type, int count);

	void spawn(EnemyType type, float xIndex);
	Enemy* getEnemy(unsigned int index);

	unsigned int size() { return mActiveEnemies.size(); }

	void render();

private:
	void deactivate(std::vector<Enemy*>::iterator& iter);

private:
	GameData* mGameData;

	std::vector<EnemyObject> mEnemyPool;
	std::vector<Enemy*> mActiveEnemies;

	ColliderManager mCollisionManager;

	EnemySpawner mSpawner;
};