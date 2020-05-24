#pragma once

#include "Items/Spawner.h"
#include "Objects/Actors/Enemies/EnemyEnums.h"

class EnemyManager;
class Shape;


class EnemySpawner : public Spawner
{
public:
	EnemySpawner(EnemyManager* enemies) : mEnemies(enemies) { }

	void spawnLevel(Map* map, int level);

	void spawnPatrollers(Map* map, int xIncrement, EnemyType type);
	void spawnShape(Map* map, int xPoint, Shape shape, EnemyType type);


private:
	void level1(Map* map);
	void level2(Map* map);
	void level3(Map* map);

	Shape pickRandomShape();

private:
	EnemyManager* mEnemies;
};