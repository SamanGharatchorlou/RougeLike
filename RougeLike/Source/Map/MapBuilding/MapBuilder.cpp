#include "pch.h"
#include "MapBuilder.h"

#include "Map/Map.h"
#include "MapGenerator.h"
#include "MapPopulator.h"
#include "MapDecorator.h"

#include "Game/Environment.h"


void MapBuilder::init(Environment* environment)
{
	mEffectPool = environment->effectPool();
	mPlayer = environment->actors()->player()->get();
}


void MapBuilder::load()
{
	BasicString path = FileManager::Get()->findFile(FileManager::Config_Map, "Environment");
	XMLParser parser(path);
	mSpecs.set(parser.rootNode());
	mSpecs.readTrapData();

	int poolSize = 5;
	for (int i = 0; i < poolSize; i++)
	{
		mPool.push(new Map);
	}
}

void MapBuilder::clear()
{
	while (mPool.size() > 0)
	{
		delete mPool.popFront();
	}

	mSpecs.clear();
}


void MapBuilder::returnMap(Map* map)
{
	map->clear();
	mPool.push(map);
}



Map* MapBuilder::buildMap(MapType type, VectorF offset)
{
	Map* map = mPool.popFront();

	buildMapStructure(map, type);
	
	fillMapData(map, type, offset, 1);
	map->initTrapManager(mPlayer, mEffectPool, mSpecs.trapData());

	return map;
}


Map* MapBuilder::buildFirst()
{
	Map* map = mPool.popFront();

	buildMapStructure(map, MapType::Corridor);

	for (int y = 0; y < map->yCount(); y++)
	{
		for (int x = 0; x < 2; x++)
		{
			Index index(x, y);
			map->tile(index)->set(CollisionTile::Wall);
			map->tile(index)->set(RenderTile::Wall);
		}
	}

	fillMapData(map, MapType::Corridor, VectorF(), 0);
	map->initTrapManager(mPlayer, mEffectPool, mSpecs.trapData());

	return map;
}


void MapBuilder::fillMapData(Map* map, MapType type, VectorF offset, int level)
{
	map->setInfo(type, level);

	addMapDecor(map);
	populateMapRects(map, offset);
	populateMapData(map);

	map->buildDeferredRenderList();
}


void MapBuilder::close(Map* map)
{
	// Carve out path
	for (int y = 0; y < map->yCount(); y++)
	{
		for (int x = 0; x < 1; x++)
		{
			Index index(x, y);
			map->tile(index)->set(RenderTile::Wall);
		}
	}

	populateMapData(map);
}


void MapBuilder::populateMapData(Map* map)
{
	MapPopulator populator;
	populator.populateData(map);
}


void MapBuilder::populateMapRects(Map* map, VectorF offset)
{
	VectorF tileSize = mSpecs.size("Tile");
	for (int y = 0; y < map->yCount(); y++)
	{
		for (int x = 0; x < map->xCount(); x++)
		{
			Index index(x, y);

			VectorF position = index.toFloat() * tileSize;
			RectF rect(position + offset, tileSize);

			map->tile(index)->setRect(rect);
		}
	}
}


void MapBuilder::buildMapStructure(Map* map, MapType type)
{
	MapGenerator generator;

	if (type == MapType::Dungeon)
	{
		VectorF size = mSpecs.size("Dungeon");
		map->setSize(size.toInt());
		generator.buildDungeon(map->getData());
	}
	else if (type == MapType::Corridor)
	{
		VectorF size = mSpecs.size("Corridor");
		map->setSize(size.toInt());
		generator.buildCorridor(map->getData());
	}
	else
		DebugPrint(Warning, "Map type %d not recognised\n");
}


void MapBuilder::addMapDecor(Map* map)
{
	const DecorMap& decorMap = mSpecs.getDecor(map);
	MapDecorator decorator;
	decorator.addDecor(map, decorMap);
}
