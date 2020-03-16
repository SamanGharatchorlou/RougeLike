#pragma once
#include "Map/MapTile.h"

class TunnelGenerator
{
public:
	TunnelGenerator();

	void buildRandom(Grid<MapTile>& mapData);
	void buildTunnel(Grid<MapTile>& mapData);
};