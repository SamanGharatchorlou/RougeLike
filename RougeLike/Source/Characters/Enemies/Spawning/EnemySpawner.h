#pragma once

#include "Items/Spawner.h"
#include "Characters/Enemies/EnemyEnums.h"

class Shape;

class EnemySpawner : public Spawner
{
public:
	EnemySpawner(EnemyManager* enemies) : mEnemies(enemies) { }

	void spawnPatrollers(Map* map, int xIncrement);

	void spawnShape(Map* map, int xPoint, Shape shape, EnemyType type);

private:
	EnemyManager* mEnemies;
};