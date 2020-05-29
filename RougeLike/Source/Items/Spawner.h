#pragma once

class Map;

class Spawner
{
public:
	VectorF findSpawnPoint(Map* map, int xPositionPercentage);

private:
	Vector2D<int> findYFloorTileRange(Map* map, int xIndex) const;
};

