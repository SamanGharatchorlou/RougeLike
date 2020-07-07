#pragma once

#include "MapTile.h"

class TextureManager;


class MapTileDecoder
{
public:
	MapTileDecoder(TextureManager* tm) : mTextureManager(tm) { }

	void populateData(Grid<MapTile>& data);


private:
	bool isValidIndex(const Index index, const Grid<MapTile>& data) const;

	// Render info
	void populateRenderInfo(Grid<MapTile>& data);

	// Base wall types
	void topBottom(Grid<MapTile>& data);
	void leftRight(Grid<MapTile>& data);
	void corners(Grid<MapTile>& data);
	void pointCorners(Grid<MapTile>& data);

	// Finishing
	void cleanLabels(Grid<MapTile>& data);
	void setTextures(Grid<MapTile>& data);

	// Columns
	void addColumns(Grid<MapTile>& data);
	bool canBeColumn(const Grid<MapTile>& data, const Index lowerIndex) const;

	// Water
	void addWater(Grid<MapTile>& data);

	// Flame tourch
	void addTourch(Grid<MapTile>& data);

	// Collision Info
	void editCollisionInfo(Grid<MapTile>& data);


private:
	TextureManager* mTextureManager;
};