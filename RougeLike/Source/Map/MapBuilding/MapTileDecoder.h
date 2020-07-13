#pragma once

#include "Map/MapTile.h"

class TextureManager;


class MapTileDecoder
{
public:
	MapTileDecoder(TextureManager* tm) : mTextureManager(tm) { }

	void populateData(Grid<MapTile>& data);

private:
	void editCollisionInfo(Grid<MapTile>& data);
	void setTextures(Grid<MapTile>& data);


private:
	TextureManager* mTextureManager;
};


