#include "pch.h"
#include "Environment.h"

#include "Game/Camera.h"
#include "TunnelGenerator.h"
#include "Map.h"


Environment::Environment(TextureManager* textureManager) : mTextureManager(textureManager), mMapLevel(1)
{
	ASSERT(Error, mTextureManager != nullptr, "You need to new the texture manager before this class\n");

	createMaps();
};



void Environment::restart()
{
	delete mEntrace;
	delete mPrimaryMap;
	delete mExit;

	createMaps();
}


void Environment::readConfigData(Vector2D<int>& mapIndexSize, VectorF& tileSize, float& scale)
{
	XMLParser parser;
	parser.parseXML(FileManager::Get()->XMLFilePath(FileManager::Config_Map, "Map"));

	xmlNode rootNode = parser.rootNode();
	xmlNode tileSetInfoNode = rootNode->first_node("TilesetInfo");
	xmlNode node = nullptr;
	Attributes attributes;

	// Map size
	node = tileSetInfoNode->first_node("TileCount");
	attributes = parser.attributes(node);

	int tileCountX = attributes.getInt("x");
	int tileCountY = attributes.getInt("y");
	mapIndexSize.set(tileCountX, tileCountY);

	// Tile size
	node = tileSetInfoNode->first_node("TileSize");
	attributes = parser.attributes(node);

	float tileSizeX = attributes.getFloat("x");
	float tileSizeY = attributes.getFloat("y");
	tileSize.set(tileSizeX, tileSizeY);

	// Scale
	node = rootNode->first_node("Scale");
	scale = std::stof(node->value());
}


void Environment::init()
{
	float offset = 0.0f;

	// create first entrace and level
	buildPassage(mEntrace, offset);

	offset = mEntrace->getLastRect().RightPoint();
	buildLevel(offset);

	offset = mPrimaryMap->getLastRect().RightPoint();
	buildPassage(mExit, offset);

	IncrementLevelEvent event;
	notify(event);
}


void Environment::nextLevel()
{
	float offset = 0.0f;

	// Set the new entrace as the old exit
	Map* oldEntrace = mEntrace;
	mEntrace = mExit;
	mExit = oldEntrace;
	mExit->clearData();

	// Build new level
	offset = mEntrace->getLastRect().RightPoint();
	buildLevel( offset);

	offset = mPrimaryMap->getLastRect().RightPoint();
	buildPassage(mExit, offset);

	mMapLevel++;
	IncrementLevelEvent event;
	notify(event);
}



void Environment::renderBottomLayer()
{
	mEntrace->renderFloor();
	mEntrace->renderTop();

	mPrimaryMap->renderFloor();
	mPrimaryMap->renderTop();

	mExit->renderFloor();
	mExit->renderTop();
}


void Environment::renderTopLayer()
{
	mEntrace->renderBottom();
	mPrimaryMap->renderBottom();
	mEntrace->renderBottom();
}


VectorF Environment::size() const
{
	return mPrimaryMap->size();
}


Map* Environment::map(VectorF position) const
{
	if (position.x > mPrimaryMap->getLastRect().LeftPoint())
	{
		return mExit;
	}
	else if (position.x < mPrimaryMap->getFirstRect().LeftPoint())
	{
		return mEntrace;
	}
	else
	{
		return mPrimaryMap;
	}
}




RectF Environment::boundaries() const
{
	float xLeft = -100.0f;
	float xRight = +10000.0f;

	//if(mExit)
	//{
	//	xLeft = mPrimaryMap->getFirstRect().LeftPoint();
	//	xRight = mExit->getLastRect().RightPoint();
	//}
	//else // mEntrace
	//{
	//	xLeft = mEntrace->getFirstRect().LeftPoint();
	//	xRight = mPrimaryMap->getLastRect().RightPoint();
	//}

	return RectF(xLeft, 0.0f, xRight, mPrimaryMap->size().y);
}


// --- Private Functions --- //
void Environment::buildLevel(float offset)
{
	mPrimaryMap->clearData();

	TunnelGenerator generator;
	generator.buildRandomA(mPrimaryMap->getData());

	mPrimaryMap->populateData(mTextureManager, VectorF(offset, 0.0f));
}


void Environment::buildPassage(Map* map, float offset)
{
	map->clearData();

	TunnelGenerator generator;
	generator.buildSimpleLine(map->getData());

	map->populateData(mTextureManager, VectorF(offset, 0.0f));
}



VectorF Environment::toWorldCoords(VectorF cameraCoords)
{
	float xOffset = mEntrace->getLastRect().RightPoint();

	VectorF cameraPosition = Camera::Get()->getTopLeftF();

	float xDiff = cameraPosition.x - xOffset;

	return cameraCoords + cameraPosition;
}


void Environment::createMaps()
{
	Vector2D<int> mapSize;
	VectorF tileSize;
	float scale = -1.0f;
	readConfigData(mapSize, tileSize, scale);

	tileSize *= scale;

	mPrimaryMap = new Map(mapSize, tileSize);

	float a = Camera::Get()->size().x;
	float b = mPrimaryMap->tileSize().x;
	mapSize.x = (int)((Camera::Get()->size().x / mPrimaryMap->tileSize().x) * 1.5f);

	mEntrace = new Map(mapSize, tileSize);
	mExit = new Map(mapSize, tileSize);
}