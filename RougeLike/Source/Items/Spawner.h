#pragma once

class Map;

// TODO: change this to only a function?
class Spawner
{
public:
	VectorF findSpawnPoint(const Map* map, int xPositionPercentage) const;

};

