#pragma once
#include "Map/Tiles/MapTile.h"
#include "Map/Tiles/TileTypes.h"



class MapGenerator
{
public:
	MapGenerator();

	void buildDungeon(Grid<MapTile>& mapData);
	void buildCorridor(Grid<MapTile>& mapData);

private:	
	void fillWithDefaultTiles(Grid<MapTile>& data);
	void setRenderTiles(Grid<MapTile>& data);

	void carveRandomTunnel(Grid<MapTile>& data, Index& outEndIndex);
	void carveStraightTunnel(Grid<MapTile>& data);

	void addExitOpening(Grid<MapTile>& data, Index opening);
};


