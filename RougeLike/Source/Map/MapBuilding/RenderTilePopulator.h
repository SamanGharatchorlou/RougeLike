#pragma once

#include "Map/MapTile.h"

class TextureManager;


class RenderTilePopulator
{
public:
	RenderTilePopulator(TextureManager* textureManager) : mTextureManager(textureManager) { }
	void populate(Grid<MapTile>& data);

private:
	void topBottom(Grid<MapTile>& data);
	void leftRight(Grid<MapTile>& data);
	void corners(Grid<MapTile>& data);
	void pointCorners(Grid<MapTile>& data);
	void cleanRenderLabels(Grid<MapTile>& data);

private:
	TextureManager* mTextureManager;
};


