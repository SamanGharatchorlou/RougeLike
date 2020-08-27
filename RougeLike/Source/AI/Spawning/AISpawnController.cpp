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


	// TODO: spawn random number of random shapes

	// spawn quad
	int xRandom = randomNumberBetween(3, 8);
	int yRandom = randomNumberBetween(2, 7);

	Quadrilateral quadFormation(VectorF(), xRandom, yRandom, 50.0f);

	mSpawnData.setRandomPosition(quadFormation, map);

	SpawnDataList spawnData = mSpawnData.buildSpawnData(quadFormation, EnemyType::Devil, EnemyState::Idle);
	merge(spawnDataList, spawnData);

	return mFactory.buildEnemies(spawnDataList, map);
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