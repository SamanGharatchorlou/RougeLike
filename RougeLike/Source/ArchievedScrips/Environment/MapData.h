#pragma once

class LevelMapping;
class MapTileset;

#if _DEBUG // Track number of MapData object are created (should only be 1)
static int mapDataCounter;
#endif

// This can now be passed around freely by value
struct MapData
{
	MapData();
	~MapData();

	LevelMapping* level;
	std::vector<MapTileset*> tilesets;

#if _DEBUG
	void initCounter() { mapDataCounter = 0; }
#endif
};