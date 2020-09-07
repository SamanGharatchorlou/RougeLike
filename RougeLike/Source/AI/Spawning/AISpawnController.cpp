#include "pch.h"
#include "AISpawnController.h"

#include "AI/Pathing/AIPathMap.h"

#include "Utilities/Shapes/Point.h"
#include "Utilities/Shapes/Quadrilateral.h"


std::vector<Enemy*> AISpawnController::getNewLevelSpawns(const AIPathMap* map)
{
	SpawnDataList spawnDataList;

	// spawn patrollers
	for (unsigned int xPoint = 5; xPoint <= 95; xPoint += 3)
	{
		VectorF position = map->randomFloorTile(xPoint)->rect().Center();
		Point patrolPoint(position);

		SpawnDataList spawnData = mSpawnData.buildSpawnData(patrolPoint, EnemyType::Devil, EnemyState::Patrol);
		merge(spawnDataList, spawnData);
	}


	for (int i = 0; i < map->level() + 2; i++)
	{
		EnemyType type = i % 2 == 0 ? EnemyType::Orc : EnemyType::Devil;
		SpawnDataList spawnData = spawnRandomQuad(type, EnemyState::Idle, map);
		merge(spawnDataList, spawnData);
	}

	return mFactory.buildEnemies(spawnDataList, map);
}


SpawnDataList AISpawnController::spawnRandomQuad(EnemyType type, EnemyState::Type state, const AIPathMap* map)
{
	int xRandom = randomNumberBetween(2, 7);
	int yRandom = randomNumberBetween(2, 7);
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


void AISpawnController::loadSpawnPool()
{
	mFactory.loadPools();
}


AISpawnController::~AISpawnController()
{
	clear();
}


void AISpawnController::clear()
{
	mFactory.clear();
}