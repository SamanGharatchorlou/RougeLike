#pragma once
#include "Map/MapTile.h"

class TunnelGenerator
{
public:
	TunnelGenerator();

	void buildRandom(Grid<MapTile>& mapData);
	void buildSimpleLine(Grid<MapTile>& mapData);

	void addExitPath(Grid<MapTile>& mapData, int startingX);
};