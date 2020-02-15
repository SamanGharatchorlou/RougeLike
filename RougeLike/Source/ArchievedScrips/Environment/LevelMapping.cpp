#include "pch.h"
#include "LevelMapping.h"


LevelMapping::Layer::Layer(Attributes attributes, std::string encoding) : id(-1)
{
	setAttributes(attributes);
	loadEncoding(encoding);
}


void LevelMapping::Layer::setAttributes(Attributes attributes)
{
	id = attributes.getInt("layer_id");

	// Tile dimentions in pixels
	tileCount.x = attributes.getInt("layer_width");
	tileCount.y = attributes.getInt("layer_height");

#if _DEBUG // get name and Log every attribute
	name = attributes.getString("layer_name");
	attributes.log();
#endif
}


void LevelMapping::Layer::loadEncoding(std::string encoding)
{
	mEncoding.clear();
	std::vector<int> encodingList = parseCSVtoInt(encoding);

	ASSERT(Warning, encodingList.size() == tileCount.x * tileCount.y, 
		"The mapping does not match up with the tile count\n");

	for (int y = 0; y < tileCount.y; y++)
	{
		// offset used to convert list into grid
		int offset = y * tileCount.x;
		std::vector<int> encodingRow(tileCount.x, -1);

		for (int x = 0; x < tileCount.x; x++)
		{
			// -1 to convert from 1 based value to zero based index
			encodingRow[x] = encodingList[offset + x] - 1;
		}

		mEncoding.push_back(encodingRow);
	}
}


// TODO: does this still make sense to do?
// ITS NOT EVEN USED ANYMORE
void LevelMapping::validateLevelData()
{
	// should I bet getting references or pointers here?
	for (auto layer : layers)
	{
		if (layer.getMapping().size() != mLevelInfo.tileCount.y && layer.getMapping()[0].size() != mLevelInfo.tileCount.x)
		{
			DebugPrint(Warning,
				"The map encoding count(%zu) does not equal the tile count (%d * %d)\n",
				layer.getMapping().size(), mLevelInfo.tileCount.x, mLevelInfo.tileCount.y);
		}
		else
		{
			DebugPrint(Log, "Level data is valid - printing data...\n");

			for (int y = 0; y < mLevelInfo.tileCount.y; y++)
			{
				for (int x = 0; x < mLevelInfo.tileCount.x; x++)
				{
					DebugPrint(Log, "%d, ", layer.getMapping()[y][x]);
				}

				DebugPrint(Log, "\n");
			}
			DebugPrint(Log, "end of data\n");
		}
	}
}


/// LEVEL
void LevelMapping::setAttributes(Attributes attributes)
{
	// Tile dimentions in pixels
	int tileWidth = attributes.getInt("map_tilewidth");
	int tileHeight = attributes.getInt("map_tileheight");
	mLevelInfo.tileDimentions = Vector2D<int>(tileWidth, tileHeight);

	// Tile count
	int xTileCount = attributes.getInt("map_width");
	int yTileCount = attributes.getInt("map_height");
	mLevelInfo.tileCount = Vector2D<int>(xTileCount, yTileCount);

	// level dimentions in pixels
	mLevelInfo.tilesetSize = mLevelInfo.tileDimentions * mLevelInfo.tileCount;

#if _DEBUG // Log and check attributes
	DebugPrint(Log, "Setting Level attributes...\n");
	attributes.log();
	mLevelInfo.validate();
#endif
}


VectorF LevelMapping::getMapDimentions()
{
	VectorF size = VectorF(mLevelInfo.tilesetSize.x, mLevelInfo.tilesetSize.y);

	//TODO: another place its needed. Place this into the Map object you need to create
	float mapScale = 3.0f;
	return size * mapScale;
}