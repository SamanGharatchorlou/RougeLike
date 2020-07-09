#pragma once

#include "MapTile.h"

class TextureManager;


class MapTileDecoder
{
public:
	MapTileDecoder(TextureManager* tm) : mTextureManager(tm) { }

	void populateData(Grid<MapTile>& data);


private:
	bool isValid(const Index index, const Grid<MapTile>& data) const;

	// Tile population
	void populateRenderInfo(Grid<MapTile>& data);
	void populateDecorInfo(Grid<MapTile>& data);
	void editCollisionInfo(Grid<MapTile>& data);
	void setTextures(Grid<MapTile>& data);

	// Render
	void topBottom(Grid<MapTile>& data);
	void leftRight(Grid<MapTile>& data);
	void corners(Grid<MapTile>& data);
	void pointCorners(Grid<MapTile>& data);
	void cleanRenderLabels(Grid<MapTile>& data);

	// Decore
	void addColumns(Grid<MapTile>& data);
	void addWater(Grid<MapTile>& data);
	void addAnimations(Grid<MapTile>& data);

private:
	TextureManager* mTextureManager;
};