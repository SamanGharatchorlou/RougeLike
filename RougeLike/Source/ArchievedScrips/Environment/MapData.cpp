#include "pch.h"
#include "MapData.h"
#include "LevelMapping.h"
#include "MapTileset.h"


MapData::MapData()
{
	level = new LevelMapping();

#if _DEBUG
	mapDataCounter++;
	ASSERT(Warning, mapDataCounter <= 1,
		"More than 1 (%d) MapData objects have been created, we only need 1!\n", mapDataCounter);
#endif
}


MapData::~MapData()
{
	delete level;
	level = nullptr;

	for (int i = 0; i < tilesets.size(); i++)
	{
		delete tilesets[i];
		tilesets[i] = nullptr;
	}

#if _DEBUG
	mapDataCounter--;
	DebugPrint(Log, "All level data deleted\n");
#endif
}