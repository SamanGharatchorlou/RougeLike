#include "pch.h"
#include "Environment.h"

#include "Game/Camera.h"
#include "MapBuilding/MapGenerator.h"
#include "Map.h"

#include "TrapManager.h"


Environment::Environment(TextureManager* textureManager) : mLevelManager(textureManager)
{
	createNewMaps();
};



void Environment::restart()
{
	delete mEntrace;
	delete mPrimaryMap;
	delete mExit;

	createNewMaps();
}


void Environment::init()
{
	mLevelManager.init(mEntrace);
	VectorF offset = mLevelManager.getOffset(mEntrace);

	mLevelManager.buildPrimary(mPrimaryMap, offset);
	offset = mLevelManager.getOffset(mPrimaryMap);

	mLevelManager.buildExit(mExit, offset);

	setCameraBoundaries();

	std::vector<SpawnData> data = mSpawner.getspawnList(mPrimaryMap, mLevelManager.level());

	int a = 4;
}


void Environment::nextLevel()
{
	// wipe enemies

	mLevelManager.incrementLevel();
	mLevelManager.swapEntranceExit(mEntrace, mExit);

	VectorF offset = mLevelManager.getOffset(mEntrace);
	mLevelManager.buildPrimary(mPrimaryMap, offset);

	offset = mLevelManager.getOffset(mPrimaryMap);
	mLevelManager.buildExit(mExit, offset);

	setCameraBoundaries();

	IncrementLevelEvent event;
	notify(event);

	// spawn new enemies

	std::vector<SpawnData> data = mSpawner.getspawnList(mPrimaryMap, mLevelManager.level());

	int a = 4;
}


void Environment::slowUpdate(float dt)
{
	mEntrace->slowUpdate(dt);
	mPrimaryMap->slowUpdate(dt);
	mExit->slowUpdate(dt);
}


void Environment::renderFloor()
{
	mEntrace->renderFloor();
	mPrimaryMap->renderFloor();
	mExit->renderFloor();
}
void Environment::renderBottomLayer()
{
	mEntrace->renderLowerLayer();
	mPrimaryMap->renderLowerLayer();
	mExit->renderLowerLayer();
}
void Environment::renderTopLayer()
{
	mEntrace->renderUpperLayer();
	mPrimaryMap->renderUpperLayer();
	mExit->renderUpperLayer();
}


VectorF Environment::size() const
{
	return mPrimaryMap->size();
}


Map* Environment::map(VectorF position) const
{
	if (position.x < mPrimaryMap->getFirstRect().LeftPoint())
		return mEntrace;
	else if (position.x < mPrimaryMap->getLastRect().RightPoint())
		return mPrimaryMap;
	else
		return mExit;
}


void Environment::setCameraBoundaries()
{
	float xLeft = mEntrace->getFirstRect().LeftPoint();
	float xRight = mExit->getLastRect().RightPoint();
	RectF boundaries(xLeft, 0.0f, xRight, mPrimaryMap->size().y);

	Camera::Get()->setMapBoundaries(boundaries);
}


bool Environment::canClosePreviousLevel(VectorF playerPosition) const
{
	float buffer = 100.0f;
	float x = mPrimaryMap->getLastRect().RightPoint() + buffer;
	float y = playerPosition.y;

	if (playerPosition.x > x)
		return !Camera::Get()->inView(VectorF(x, y));
	else
		return false;
}



// --- Private Functions --- //
void Environment::readConfigData(Vector2D<int>& mapIndexSize, VectorF& tileSize, float& scale)
{
	XMLParser parser;
	std::string path = FileManager::Get()->findFile(FileManager::Config_Map, "Map");
	parser.parseXML(path);

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


VectorF Environment::toWorldCoords(VectorF cameraCoords)
{
	float xOffset = mEntrace->getLastRect().RightPoint();

	VectorF cameraPosition = Camera::Get()->rect().TopLeft();

	float xDiff = cameraPosition.x - xOffset;

	return cameraCoords + cameraPosition;
}


void Environment::createNewMaps()
{
	Vector2D<int> primaryMapSize;
	VectorF tileSize;
	float scale = -1.0f;
	readConfigData(primaryMapSize, tileSize, scale);

	tileSize *= scale;

	mPrimaryMap = new Map(primaryMapSize, tileSize);


	Vector2D<int> corridorMapSize;
	corridorMapSize.x = (int)((Camera::Get()->size().x / mPrimaryMap->tileSize().x) * 1.5f);
	corridorMapSize.y = primaryMapSize.y;

	mEntrace = new Map(corridorMapSize, tileSize);
	mExit = new Map(corridorMapSize, tileSize);
}