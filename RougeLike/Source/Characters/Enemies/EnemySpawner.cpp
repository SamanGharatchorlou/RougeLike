#include "pch.h"
#include "EnemySpawner.h"

#include "Map/Map.h"
#include "Enemy.h"

// Find a random valid y tile at map position x
void EnemySpawner::spawn(Enemy* enemy, float xPositionPercentage)
{
	float xPercent = xPositionPercentage / 100.0f;
	ASSERT(Warning, xPercent > 0 && xPercent < 1.0f,
		"Invalid position percentage: %f. Enemy must be spawned between 0 and 1\n", xPercent);

	int xTileIndex = (int)(mMap->xCount() * xPercent);

	Vector2D<int> yTileRange = mMap->findYFloorTileRange(xTileIndex);

	ASSERT(Warning, yTileRange.x > 0 && yTileRange.y < (int)mMap->yCount(),
		"No valid tile was found to spawn enemy at x = %d\n", xTileIndex);

	// Use [1] -1 becuase we don't want it to be hiding behind the wall
	int randomYTile = randomNumberBetween(yTileRange.x, yTileRange.y - 1);
	ASSERT(Warning, mMap->floorRenderTile(xTileIndex, randomYTile), "Cannot spawn enemy on a wall tile\n");

	enemy->spawn(mMap->getTileRect(xTileIndex, randomYTile).Center());
	DebugPrint(Log, "Spawing enemy at %f,%f\n",
		mMap->getTileRect(xTileIndex, randomYTile).Center().x, mMap->getTileRect(xTileIndex, randomYTile).Center().y);
}