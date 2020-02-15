#pragma once
#include "MapTileset.h"
#include "Utilities/XMLParser.h"

class MapData;
class TextureManager;

class MapDecoder
{
public:
	MapDecoder(TextureManager* textureManager, MapData* mapData);
	~MapDecoder() { }

	void decodeMap(std::string levelDataFilePath);

	void loadLevelData();
	void loadTilesetData(MapTileset* tileSet);

private:
	void generateTilesets(xmlNode mapNode);
	void generateMapLayers(xmlNode mapNode);

private:
	TextureManager* mTextureManager;
	MapData* mMap;

	XMLParser xmlParser;
};
