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
	void addTorchHandles(Grid<MapTile>& data);
	void addSpikes(Grid<MapTile>& data);

private:	
	void fillWithDefaultTiles(Grid<MapTile>& data);
	void setRenderTiles(Grid<MapTile>& data);

	void carveRandomTunnel(Grid<MapTile>& data, Index& outEndIndex);
	void carveStraightTunnel(Grid<MapTile>& data);

	void addExitOpening(Grid<MapTile>& data, Index opening);

	bool isValid(const Index index, const Grid<MapTile>& data) const;

	bool canAddColumn(const Grid<MapTile>& data, Index lowerIndex, int columnWidth) const;
	bool canAddWater(const Grid<MapTile>& data, Index index, Vector2D<int> size) const;
};