#pragma once

#include "Map/Tiles/MapTile.h"

class TextureManager;
class Map;

class MapPopulator
{
public:
	void populateData(Map* map, const TextureManager* textures);

private:
	void editCollisionInfo(Grid<MapTile>& data);
	void setTextures(Grid<MapTile>& data, const TextureManager* textures);
};


