#include "pch.h"
#include "MapSpecifications.h"

#include "Map/Map.h"


void MapSpecifications::readTrapData()
{
	FileManager* fm = FileManager::Get();
	std::vector<BasicString> filePaths = fm->allFilesInFolder(FileManager::Configs_MapObjects);

	for (const BasicString& path : filePaths)
	{
		XMLParser parser(path);
		XMLNode root = parser.rootNode();
		XMLNode infoNode = root.child("Info");

		if (infoNode)
		{
			const BasicString fileName = fm->getItemName(path);
			DecorType type = stringToDecorType(fileName);

			PropertyMap properties(infoNode);

			mTrapData[type] = properties;
		}
	}
}


void MapSpecifications::set(const XMLNode node)
{
	readTileSize(node);
	readMapSize(node);
	fillSpecifications(node);
}

void MapSpecifications::clear()
{
	mSizes.clear();
	mDungeonDecor.empty();
	mCorridorDecor.empty();
}


void MapSpecifications::readTileSize(const XMLNode root)
{
	XMLNode tileSizeNode = root.child("TileSize");
	StringMap attributes = tileSizeNode.attributes();
	float scale = toFloat(root.child("Scale").value());
	mSizes["Tile"] = attributes.getVector("x", "y") * scale;
}


void MapSpecifications::fillSpecifications(const XMLNode node)
{
	XMLNode decorNode = node.child("Decor");

	XMLNode dungeonNode = decorNode.child("Dungeon");
	mDungeonDecor.fill(dungeonNode);

	XMLNode corridorNode = decorNode.child("Corridor");
	mCorridorDecor.fill(corridorNode);
}


void MapSpecifications::readMapSize(const XMLNode node)
{
	XMLNode corridorSizeNode = node.child("CorridorSize");
	StringMap corridorAttributes = corridorSizeNode.attributes();
	mSizes["Corridor"] = corridorAttributes.getVector("x", "y");

	XMLNode dungeonSizeNode = node.child("DungeonSize");
	StringMap dungeonAttributes = dungeonSizeNode.attributes();
	mSizes["Dungeon"] = dungeonAttributes.getVector("x", "y");
}

const DecorMap& MapSpecifications::getDecor(const Map* map)
{
	if (map->type() == MapType::Corridor)
		return mCorridorDecor;
	else
		return mDungeonDecor;
}