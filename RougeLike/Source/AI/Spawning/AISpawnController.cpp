#include "pch.h"
#include "AISpawnController.h"

#include "AI/Pathing/AIPathMap.h"

#include "Utilities/Shapes/Point.h"
#include "Utilities/Shapes/Quadrilateral.h"


std::vector<Enemy*> AISpawnController::getNewLevelSpawns(const AIPathMap* map)
{
	SpawnDataList spawnDataList;

	// spawn patrollers
	for (unsigned int xPoint = 5; xPoint <= 95; xPoint += 5)
	{
		VectorF position = map->randomFloorTile(xPoint)->rect().Center();
		Point patrolPoint(position);

		SpawnDataList spawnData = mSpawnData.buildSpawnData(patrolPoint, EnemyType::Devil, EnemyState::Patrol);
		merge(spawnDataList, spawnData);
	}


	for (int i = 0; i < map->level(); i++)
	{
		SpawnDataList spawnData = spawnRandomQuad(EnemyType::Devil, EnemyState::Idle, map);
		merge(spawnDataList, spawnData);
	}

	return mFactory.buildEnemies(spawnDataList, map);
}


SpawnDataList AISpawnController::spawnRandomQuad(EnemyType type, EnemyState::Type state, const AIPathMap* map)
{
	int xRandom = randomNumberBetween(2, 8);
	int yRandom = randomNumberBetween(2, 8);
	Quadrilateral formation(VectorF(), xRandom, yRandom, 40.0f);
	setRandomPosition(formation, map);

	return mSpawnData.buildSpawnData(formation, type, state);
}


void AISpawnController::setRandomPosition(Formation& formation, const AIPathMap* map)
{
	while (!formation.isValid(map))
	{
		VectorF randomFloorPosition = map->randomFloorTile()->rect().Center();
		formation.setPosition(randomFloorPosition);
	}
}


void AISpawnController::returnEnemy(Enemy* enemy)
{
	mFactory.returnEnemy(enemy);
}


void AISpawnController::loadSpawnPool(int poolSize)
{
	mFactory.loadPools(poolSize);
}


AISpawnController::~AISpawnController()
{
	clear();
}


void AISpawnController::clear()
{
	mFactory.clear();
}