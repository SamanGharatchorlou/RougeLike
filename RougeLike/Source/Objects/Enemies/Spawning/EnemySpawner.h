#pragma once

#include "Items/Spawner.h"
#include "Objects/Enemies/EnemyEnums.h"

class EnemyManager;
class Shape;


class EnemySpawner : public Spawner
{
public:
	EnemySpawner(EnemyManager* enemies) : mEnemies(enemies) { }

	void spawnLevel(Map* map, int level);

	void spawnPatrollers(Map* map, int xIncrement);
	void spawnShape(Map* map, int xPoint, Shape shape, EnemyType type);


private:
	Shape pickRandomShape();

private:
	EnemyManager* mEnemies;
};