#pragma once

#include "MapTile.h"

class TextureManager;


class MapTileDecoder
{
public:
	MapTileDecoder(TextureManager* tm) : mTextureManager(tm) { }

	void populateData(Grid<MapTile>& data);


private:
	bool isValidIndex(Index index, Grid<MapTile>& data) const;

	// Render info
	void fillRenderInfo(Grid<MapTile>& data);
	void topBottom(Grid<MapTile>& data);
	void leftRight(Grid<MapTile>& data);
	void corners(Grid<MapTile>& data);
	void pointCorners(Grid<MapTile>& data);
	void cleanLabels(Grid<MapTile>& data);
	void setTextures(Grid<MapTile>& data);


	// Collision Info
	void fillCollisionInfo(Grid<MapTile>& data);


private:
	TextureManager* mTextureManager;
};