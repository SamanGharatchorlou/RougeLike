#include "pch.h"
#include "Spawner.h"

#include "Map/Map.h"


// Find a random valid y tile at map position x
VectorF Spawner::findSpawnPoint(const Map* map, int xPositionPercentage)
{
	int xTileIndex = (int)((map->xCount() * xPositionPercentage) / 100);
	ASSERT(Warning, map->isValidIndex(Index(xTileIndex,0)),
		"Invalid position percentage: %d. Enemy must be spawned between 0 and 100\n", xPositionPercentage);

	std::vector<Index> floorIndexes;
	for (int y = 0; y < map->yCount(); y++)
	{
		Index index(xTileIndex, y);
		if (map->tile(index)->is(CollisionTile::Floor))
			floorIndexes.push_back(index);
	}

	int randomIndex = randomNumberBetween(0, floorIndexes.size());
	Index index(floorIndexes[randomIndex]);
	return map->tile(index)->rect().Center();
}
