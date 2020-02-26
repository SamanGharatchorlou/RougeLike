#include "pch.h"
#include "EnemySpawner.h"

#include "Map/Map.h"
#include "Enemy.h"

// Find a random valid y tile at map position x
void EnemySpawner::spawn(Map* map, Enemy* enemy, int xPositionPercentage)
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

	enemy->spawn(map->getTileRect(xTileIndex, randomYTile).Center());

	DebugPrint(Log, "Spawing enemy at %f,%f\n",
		map->getTileRect(xTileIndex, randomYTile).Center().x, map->getTileRect(xTileIndex, randomYTile).Center().y);
}