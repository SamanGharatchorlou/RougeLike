#pragma once
#include "Map/MapTile.h"

class TunnelGenerator
{
public:
	TunnelGenerator();

	void buildRandom(Grid<MapTile>& mapData, int width);
	void buildExit(Grid<MapTile>& mapData, int width);

	void addExitPath(Grid<MapTile>& mapData, int startingX);
};