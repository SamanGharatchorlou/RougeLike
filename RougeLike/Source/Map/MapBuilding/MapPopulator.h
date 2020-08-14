#pragma once

#include "Map/Tiles/MapTile.h"

class Map;

class MapPopulator
{
public:
	void populateData(Map* map);

private:
	void editCollisionInfo(Grid<MapTile>& data);
	void setTextures(Grid<MapTile>& data);
};


