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
	void fillRenderInfo(Grid<MapTile>& data);

	

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
	bool canAddWater(const Grid<MapTile>& data, const Index index, Vector2D<int> size) const;

	// Flame tourch
	void addTourch(Grid<MapTile>& data);

	// Collision Info
	void fillCollisionInfo(Grid<MapTile>& data);


private:
	TextureManager* mTextureManager;
};