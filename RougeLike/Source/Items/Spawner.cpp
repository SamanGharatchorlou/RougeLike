#include "pch.h"
#include "Spawner.h"

#include "Map/Map.h"

// Find a random valid y tile at map position x
VectorF Spawner::findSpawnPoint(Map* map, int xPositionPercentage)
{

	ASSERT(Warning, xPositionPercentage >= 0 && xPositionPercentage <= 100,
		"Invalid position percentage: %d. Enemy must be spawned between 0 and 100\n", xPositionPercentage);

	int xTileIndex = (int)((map->xCount() * xPositionPercentage) / 100);

	Vector2D<int> yTileRange = findYFloorTileRange(map, xTileIndex);

	ASSERT(Warning, yTileRange.x > 0 && yTileRange.y < (int)map->yCount(),
		"No valid tile was found to spawn enemy at x = %d\n", xTileIndex);

	// Use [1] becuase we don't want it to be hiding behind the wall
	int randomYTile = randomNumberBetween(yTileRange.x, yTileRange.y);

	//ASSERT(Warning, map->floorCollisionTile(Index(xTileIndex, randomYTile)), "Cannot spawn enemy on a wall tile\n");

	Index index(xTileIndex, randomYTile);
	return map->tile(index)->rect().Center();
	return VectorF();
}



Vector2D<int> Spawner::findYFloorTileRange(Map* map, int xIndex) const
{
	int yTileIndex = 0;
	Vector2D<int> yTileRange;

	bool isWall = true;
	while(isWall)
	{
		Index index(xIndex, ++yTileIndex);
		isWall = map->tile(index)->is(CollisionTile::Wall);
	}

	// Top
	yTileRange.x = yTileIndex;

	while (!isWall)
	{
		if (yTileIndex == map->yCount() - 1)
			break;

		Index index(xIndex, ++yTileIndex);
		isWall = map->tile(index)->is(CollisionTile::Wall);
	}

	yTileRange.y = yTileIndex;
	return yTileRange;
}