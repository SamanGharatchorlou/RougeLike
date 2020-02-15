#include "pch.h"
#include "Environment/MapTileset.h"
#include "Utilities/FileManager.h"

void MapTileset::init(Texture* tileSetTexture)
{
	mTexture = tileSetTexture;

#if _DEBUG // Tileset Info needs to be setup before dividing into tiles
	ASSERT(Warning, !mAttributes.empty(), "The tileset attributes have not been setup yet\n");
	mTilesetInfo.validate();
#endif

	divideIntoTiles();
}

void MapTileset::setAttributes(Attributes tilesetAttributes)
{
	mAttributes = tilesetAttributes;

	// Tile dimentions in pixels
	int tileWidth = mAttributes.getInt("tileset_tilewidth");
	int tileHeight = mAttributes.getInt("tileset_tileheight");
	mTilesetInfo.tileDimentions = Vector2D<int>(tileWidth, tileHeight);

	// Tileset dimentions in pixels
	int xTilesetWidth = mAttributes.getInt("image_width");
	int yTilesetWidth = mAttributes.getInt("image_height");
	mTilesetInfo.tilesetSize = Vector2D<int>(xTilesetWidth, yTilesetWidth);

	// Tile count
	int totalTileCount = mAttributes.getInt("tileset_tilecount");
	int xTileCount = mAttributes.getInt("tileset_columns");
	int yTileCount = totalTileCount / xTileCount;
	mTilesetInfo.tileCount = Vector2D<int>(xTileCount, yTileCount);

#if _DEBUG // Log and check attributes
	mAttributes.log();
	mTilesetInfo.validate();
#endif
}


std::string MapTileset::getTilesetDataPath()
{
	std::string source = mAttributes.getString("tileset_source");
	return FileManager::Get()->getFilePath(FileManager::Config_Map, source);
}