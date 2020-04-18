#pragma once

class Map;
class Enemy;
class EnemyManager;


class ItemSpawner
{
public:
	VectorF findSpawnPoint(Map* map, int xPositionPercentage);
};


class Spawner
{
public:
	Spawner(EnemyManager* enemies) : mEnemies(enemies) { }


	VectorF findSpawnPoint(Map* map, int xPositionPercentage);

	void spawnLevelPatrollers(Map* map);

private:
	EnemyManager* mEnemies;
};