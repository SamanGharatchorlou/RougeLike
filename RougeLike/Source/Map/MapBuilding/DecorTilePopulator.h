#pragma once

#include "Map/MapTile.h"

class TextureManager;


class DecorTilePopulator
{
public:
	DecorTilePopulator(TextureManager* textureManager) : mTextureManager(textureManager) { }
	void populate(Grid<MapTile>& data);

private:
	void addColumns(Grid<MapTile>& data);
	void addWater(Grid<MapTile>& data);
	void addAnimations(Grid<MapTile>& data);

private:
	TextureManager* mTextureManager;
};


