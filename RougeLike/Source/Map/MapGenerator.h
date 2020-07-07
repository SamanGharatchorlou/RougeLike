#pragma once
#include "Map/MapTile.h"

class MapGenerator
{
public:
	MapGenerator();

	void buildDungeon(Grid<MapTile>& mapData);
	void buildCorridor(Grid<MapTile>& mapData);

	void addWaterFeatures(Grid<MapTile>& data);
	void addTorches(Grid<MapTile>& data);


private:	
	void prepareMap(Grid<MapTile>& data);
	void completeBuild(Grid<MapTile>& data);

	bool isValidIndex(const Index index, const Grid<MapTile>& data) const;

	bool canAddWater(const Grid<MapTile>& data, const Index index, Vector2D<int> size) const;
};