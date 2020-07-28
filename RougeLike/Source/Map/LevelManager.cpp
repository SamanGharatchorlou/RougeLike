#include "pch.h"
#include "LevelManager.h"

#include "TileTypes.h"
#include "Map.h"

#include "MapBuilding/MapGenerator.h"
#include "MapBuilding/MapDecorator.h"


void LevelManager::init(Map* map)
{
	map->clearData();
	MapGenerator generator;
	generator.buildCorridor(map->getData());
	map->populateData(mTextureManager, VectorF(0,0));
}

void LevelManager::buildPrimary(Map* map, VectorF offset)
{
	map->clearData();

	MapGenerator generator;
	generator.buildDungeon(map->getData());

	decorate(map, "Primary");

	map->populateData(mTextureManager, offset);
}

void LevelManager::buildExit(Map* map, VectorF offset)
{
	map->clearData();

	MapGenerator generator;
	generator.buildCorridor(map->getData());

	decorate(map, "Exit");

	map->populateData(mTextureManager, offset);
}

VectorF LevelManager::getOffset(const Map* map) const
{
	return VectorF(map->getLastRect().RightPoint(), 0.0f);
}


void LevelManager::swapEntranceExit(Map*& entrance, Map*& exit)
{
	Map* oldEntrace = entrance;
	entrance = exit;
	exit = oldEntrace;
}


// Decorations
void LevelManager::decorate(Map* map, const std::string& section)
{
	std::string fileName = "Level" + std::to_string(mLevel);
	std::string path = FileManager::Get()->findFile(FileManager::Config_Map, fileName);

	if (!path.empty())
	{
		// Decorations
		XMLParser parser(path);
		DecorMap decorations = getLevelDecorInfo(parser, section);
		MapDecorator decorator;
		decorator.addDecor(decorations, map->getData());

		// Traps
		DecorMap trapInfo = getLevelTrapInfo(parser, section);
		setTrapInfo(map, trapInfo);
	}
}

DecorMap LevelManager::getLevelDecorInfo(XMLParser& parser, const std::string& section)
{
	DecorMap decorMap;
	xmlNode rootNode = parser.rootNode();
	xmlNode sectionNode = rootNode->first_node(section.c_str());

	if (sectionNode)
	{
		xmlNode decorNode = sectionNode->first_node("Decor");
		xmlNode decorItemNode = decorNode->first_node();
		while (decorItemNode)
		{
			DecorType type = stringToType(decorItemNode->name());
			Attributes attributes = parser.attributes(decorItemNode);
			decorMap[type] = attributes;

			decorItemNode = decorItemNode->next_sibling();
		}
	}
	else
		DebugPrint(Log, "Not a valid section '%s'\n", section.c_str());

	return decorMap;
}


// Traps
void LevelManager::setTrapInfo(Map* map, DecorMap& trapInfo)
{
	if (trapInfo.count(DecorType::Spikes))
	{
		const Attributes attributes = trapInfo[DecorType::Spikes];
		if (attributes.contains("triggerTime") && attributes.contains("recoveryTime"))
		{
			Damage damage = attributes.getFloat("damage");
			float triggerTime = attributes.getFloat("triggerTime");
			float recoveryTime = attributes.getFloat("recoveryTime");
			map->traps().set(damage, triggerTime, recoveryTime);
		}
	}
}


DecorMap LevelManager::getLevelTrapInfo(XMLParser& parser, const std::string& section)
{
	DecorMap decorMap;
	xmlNode rootNode = parser.rootNode();
	xmlNode sectionNode = rootNode->first_node(section.c_str());

	if (sectionNode)
	{
		xmlNode trapNode = sectionNode->first_node("Traps");
		if (trapNode)
		{
			xmlNode spikesNode = trapNode->first_node("Spikes");
			if (spikesNode)
			{
				DecorType type = stringToType(spikesNode->name());
				Attributes attributes = parser.attributes(spikesNode);
				decorMap[type] = attributes;
			}
		}
	}
	else
		DebugPrint(Log, "Not a valid section '%s'\n", section.c_str());

	return decorMap;
}