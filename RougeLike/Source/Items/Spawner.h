#pragma once

class Map;
class Enemy;
class EnemyManager;


class Spawner
{
public:
	VectorF findSpawnPoint(Map* map, int xPositionPercentage);
};


class EnemySpawner : public Spawner
{
public:
	EnemySpawner(EnemyManager* enemies) : mEnemies(enemies) { }

	void spawnLevelPatrollers(Map* map, int xIncrement);

private:
	EnemyManager* mEnemies;
};