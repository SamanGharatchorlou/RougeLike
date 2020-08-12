#include "pch.h"
#include "MapBuilder.h"

#include "Map/Map.h"
#include "MapGenerator.h"
#include "MapPopulator.h"
#include "MapDecorator.h"


MapBuilder::MapBuilder(TextureManager* textures) : mTextures(textures) 
{ 
	int poolSize = 5;
	for (int i = 0; i < poolSize; i++)
	{
		mPool.push(new Map());
	}

	BasicString path = FileManager::Get()->findFile(FileManager::Config_Map, "Environment");
	XMLParser parser(path);
	mSpecs.set(parser.rootNode());
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
	addMapDecor(map);
	populateMapRects(map, offset);
	populateMapData(map);
	addTrapMechanics(map);

	return map;
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
	populator.populateData(map, mTextures);
}


void MapBuilder::populateMapRects(Map* map, VectorF offset)
{
	VectorF tileSize = mSpecs.size("Tile");
	for (int y = 0; y < map->yCount(); y++)
	{
		for (int x = 0; x < map->xCount(); x++)
		{
			Index index(x, y);

			VectorF position = index * tileSize;
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
		map->setSize(size);
		generator.buildDungeon(map->getData());
	}
	else if (type == MapType::Corridor)
	{
		VectorF size = mSpecs.size("Corridor");
		map->setSize(size);
		generator.buildCorridor(map->getData());
	}
	else
		DebugPrint(Warning, "Map type %d not recognised\n");

	map->setType(type);
}


void MapBuilder::addMapDecor(Map* map)
{
	const DecorMap& decorMap = mSpecs.getDecor(map);
	MapDecorator decorator;
	decorator.addDecor(map, decorMap);
}


void MapBuilder::addTrapMechanics(Map* map)
{
	//const DecorMap& decorMap = mSpecs.getDecor(map);
	//map->traps().set(&decorMap);

	//if (decorMap.contains(DecorType::Spikes))
	//{
	//	const StringMap& attributes = decorMap.at(DecorType::Spikes);
	//	if (attributes.contains("triggerTime") && attributes.contains("recoveryTime"))
	//	{
	//		Damage damage = attributes.getFloat("damage");
	//		float triggerTime = attributes.getFloat("triggerTime");
	//		float recoveryTime = attributes.getFloat("recoveryTime");
	//		map->traps().set(damage, triggerTime, recoveryTime);
	//	}
	//}

	//if (decorMap.contains(DecorType::Trigger))
	//{
	//	const StringMap& attributes = decorMap.at(DecorType::Trigger);

	//	if (attributes.contains("triggerTime") && attributes.contains("recoveryTime"))
	//	{
	//		Damage damage = 0;
	//		float triggerTime = attributes.getFloat("triggerTime");
	//		float recoveryTime = attributes.getFloat("recoveryTime");
	//		map->traps().set(damage, triggerTime, recoveryTime);
	//	}
	//}
}



