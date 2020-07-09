#pragma once
#include "Map/MapTile.h"

class MapGenerator
{
public:
	MapGenerator();

	void buildDungeon(Grid<MapTile>& mapData);
	void buildCorridor(Grid<MapTile>& mapData);

	void addColumns(Grid<MapTile>& data);
	void addWaterFeatures(Grid<MapTile>& data);
	void addTorches(Grid<MapTile>& data);
	void addSpikes(Grid<MapTile>& data);

private:	
	void prepareMap(Grid<MapTile>& data);
	void completeBuild(Grid<MapTile>& data);

	bool isValid(const Index index, const Grid<MapTile>& data) const;

	bool canAddColumn(const Grid<MapTile>& data, Index lowerIndex, int columnWidth) const;
	bool canAddWater(const Grid<MapTile>& data, Index index, Vector2D<int> size) const;
};