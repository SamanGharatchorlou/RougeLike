#pragma once
#include "Map/MapTile.h"

class TunnelGenerator
{
public:
	TunnelGenerator(Grid<MapTile>& mapData);
	void build();

private:
	Grid<MapTile>& map;
};