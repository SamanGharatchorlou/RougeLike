#include "pch.h"
#include "AISpawnController.h"

#include "AI/Pathing/AIPathMap.h"

#include "Utilities/Shapes/Point.h"
#include "Utilities/Shapes/Quadrilateral.h"


std::vector<Enemy*> AISpawnController::getNewLevelSpawns(const AIPathMap* map)
{
	SpawnDataList spawnDataList;

	// spawn patrollers
	int spacing = getPatrollerSpacing(map->level());
	
	for (unsigned int xPoint = 5; xPoint <= 95; xPoint += spacing)
	{
		VectorF position = map->randomFloorTile(xPoint)->rect().Center();
		Point patrolPoint(position);

		SpawnDataList spawnData = mSpawnData.buildSpawnData(patrolPoint, EnemyType::Devil, EnemyState::Patrol);
		merge(spawnDataList, spawnData);
	}


	for (int i = 0; i < map->level() + 1; i++)
	{
		EnemyType type = i % 2 == 0 ? EnemyType::Orc : EnemyType::Devil;
		SpawnDataList spawnData = spawnRandomQuad(type, EnemyState::Idle, map);
		merge(spawnDataList, spawnData);
	}

	return mFactory.buildEnemies(spawnDataList, map);
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


// -- Private Functions -- //

SpawnDataList AISpawnController::spawnRandomQuad(EnemyType type, EnemyState::Type state, const AIPathMap* map)
{
	Vector2D<int> formationSize = getRandomFormationSize(map->level());
	Quadrilateral formation(VectorF(), formationSize.x, formationSize.y, 40.0f);
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


int AISpawnController::getPatrollerSpacing(int mapLevel) const
{
	int spacing = 5;

	if (mapLevel > 2)  spacing = 4;
	if (mapLevel > 5)  spacing = 3;
	if (mapLevel > 7)  spacing = 2;
	if (mapLevel > 10) spacing = 1;

	return spacing;
}


Vector2D<int> AISpawnController::getRandomFormationSize(int mapLevel) const
{
	int minimun = 2;
	int maximum = 5 + mapLevel;

	int xRandom = randomNumberBetween(minimun, maximum);
	int yRandom = randomNumberBetween(minimun, maximum);

	return Vector2D<int>(xRandom, yRandom);
}
