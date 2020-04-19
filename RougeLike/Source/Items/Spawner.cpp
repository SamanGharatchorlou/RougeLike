#include "pch.h"
#include "Spawner.h"

#include "Characters/Enemies/EnemyManager.h"
#include "Characters/Enemies/Enemy.h"
#include "Map/Map.h"

// Find a random valid y tile at map position x
VectorF Spawner::findSpawnPoint(Map* map, int xPositionPercentage)
{
	ASSERT(Warning, xPositionPercentage >= 0 && xPositionPercentage <= 100,
		"Invalid position percentage: %d. Enemy must be spawned between 0 and 100\n", xPositionPercentage);

	int xTileIndex = (int)((map->xCount() * xPositionPercentage) / 100);

	Vector2D<int> yTileRange = map->findYFloorTileRange(xTileIndex);

	ASSERT(Warning, yTileRange.x > 0 && yTileRange.y < (int)map->yCount(),
		"No valid tile was found to spawn enemy at x = %d\n", xTileIndex);

	// Use [1] -1 becuase we don't want it to be hiding behind the wall
	int randomYTile = randomNumberBetween(yTileRange.x, yTileRange.y - 1);

	ASSERT(Warning, map->floorRenderTile(xTileIndex, randomYTile), "Cannot spawn enemy on a wall tile\n");

	return map->tileRect(xTileIndex, randomYTile).Center();
}


void Spawner::spawnLevelPatrollers(Map* map)
{
	// progress in increments of 5%
	int xProgress = 0;
	int increment = 5;

	for (unsigned int xPoint = increment; xPoint < 100 - increment; xPoint += increment)
	{
		VectorF position = findSpawnPoint(map, xPoint);
		mEnemies->spawn(EnemyType::Imp, EnemyState::Patrol, position);
	}

	//VectorF position = findSpawnPoint(map, 25);
	//mEnemies->spawn(EnemyType::Imp, EnemyState::Patrol, position);
}
