#include "pch.h"
#include "MapDecoder.h"

#include "Environment/LevelMapping.h"
#include "Environment/MapTileset.h"
#include "Graphics/TextureManager.h"
#include "MapData.h"



MapDecoder::MapDecoder(TextureManager* textureManager, MapData* mapData)
	: mTextureManager(textureManager), mMap(mapData) { }


void MapDecoder::decodeMap(std::string levelDataFilePath)
{
	// Setup the parser for level data
	xmlParser.parseXML(levelDataFilePath);

	// Load in the level mapping
	loadLevelData();

	// Load in the tileset data
	for (int i = 0; i < mMap->tilesets.size(); i++)
	{
		// Setup parser for tileset data
		std::string tilesetDataFilePath = mMap->tilesets[i]->getTilesetDataPath();
		xmlParser.parseXML(tilesetDataFilePath);

		// Setup tileset
		loadTilesetData(mMap->tilesets[i]);

		Texture* tileSetTexture = mTextureManager->getTexture("Tileset");
		mMap->tilesets[i]->init(tileSetTexture);
	}

	VectorF size = mMap->level->getMapDimentions();

	DebugPrint(Log, "Map decoding complete\n");
}


void MapDecoder::loadLevelData()
{
	// Map node (root)
	xmlNode mapNode = xmlParser.getRootNode();
	ASSERT(Error, strcmp(mapNode->name(), "map") == 0, "Incorrect attributes node\n");
	Attributes mapAttributes = xmlParser.getNodeAttributes(mapNode);
	mMap->level->setAttributes(mapAttributes);

	generateTilesets(mapNode);
	generateMapLayers(mapNode);
}


void MapDecoder::loadTilesetData(MapTileset* tileSet)
{
	// Tileset node (root)
	xmlNode tilesetNode = xmlParser.getRootNode();
	ASSERT(Error, strcmp(tilesetNode->name(), "tileset") == 0, "Incorrect root node\n");
	Attributes tilesetAttributes = xmlParser.getNodeAttributes(tilesetNode);

	// Image node
	xmlNode tileSetNode = tilesetNode->first_node("image");
	ASSERT(Error, tileSetNode != nullptr, "Incorrect tile set node\n");
	Attributes imageAttributes = xmlParser.getNodeAttributes(tileSetNode);

	tilesetAttributes.merge(imageAttributes);
	tileSet->setAttributes(tilesetAttributes);
}


void MapDecoder::generateTilesets(xmlNode mapNode)
{
	// Tileset node(s)
	xmlNode tileSetNode = mapNode->first_node("tileset");
	ASSERT(Error, tileSetNode != nullptr, "Must have at least 1 tile set node\n");

	while (tileSetNode != nullptr)
	{
		Attributes tilesetAttributes = xmlParser.getNodeAttributes(tileSetNode);
		mMap->tilesets.push_back(new MapTileset(tilesetAttributes));

		tileSetNode = tileSetNode->next_sibling("tileset");
	}
}


void MapDecoder::generateMapLayers(xmlNode mapNode)
{
	// Layer node(s)
	xmlNode layerNode = mapNode->first_node("layer");
	ASSERT(Error, layerNode != nullptr, "Must have at least 1 layer node\n");

	while (layerNode != nullptr)
	{
		Attributes layerAttributes = xmlParser.getNodeAttributes(layerNode);

		// Data node
		xmlNode dataNode = layerNode->first_node();
		ASSERT(Error, dataNode != nullptr, "Incorrect data node\n");

		// Extract data
		std::string data = xmlParser.getNodeData(dataNode);

		mMap->level->add(LevelMapping::Layer(layerAttributes, data));

		layerNode = layerNode->next_sibling("layer");
	}
}
