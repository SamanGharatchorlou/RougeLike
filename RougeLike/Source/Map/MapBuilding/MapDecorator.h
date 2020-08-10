#pragma once

#include "Map/MapTile.h"
#include "Map/TileTypes.h"

class MapDecorator
{
public:
	void addDecor(DecorMap decorMap, Grid<MapTile>& data);

private:
	void addColumns(Grid<MapTile>& data, const StringMap& attributes);
	void addWaterFeatures(Grid<MapTile>& data, const StringMap& attributes);
	void addTorches(Grid<MapTile>& data, const StringMap& attributes);
	void addSpikes(Grid<MapTile>& data, const StringMap& attributes);

	bool doesTypeContain(DecorType baseType, DecorType queryType) const;


	bool canAddWater(const Grid<MapTile>& data, Index index, Vector2D<int> size) const;

	bool canAddWallDecor(const Grid<MapTile>& data, DecorType decor, Index lowerIndex, int spacing) const;
	bool isLowerWall(const Grid<MapTile>& data, Index index) const;
};

