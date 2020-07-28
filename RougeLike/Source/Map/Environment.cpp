#include "pch.h"
#include "Environment.h"

#include "Game/GameData.h"
#include "Game/Camera.h"
#include "Input/InputManager.h"
#include "MapBuilding/MapGenerator.h"
#include "Map.h"

#include "TrapManager.h"


Environment::Environment(GameData* mGameData) : 
	mLevelManager(mGameData->textureManager), mActors(mGameData)
{
};



void Environment::restart()
{
	delete mMaps.entrance;
	delete mMaps.primaryMap;
	delete mMaps.exit;

	createNewMaps();
}

void Environment::init()
{
	mActors.init(this);
}


void Environment::load()
{
	DebugPrint(Log, "\n--- Loading Environment ---\n\n");

	DebugPrint(Log, "\n Loading Maps\n");
	createNewMaps();
	mLevelManager.init(mMaps.entrance);
	VectorF offset = mLevelManager.getOffset(mMaps.entrance);

	mLevelManager.buildPrimary(mMaps.primaryMap, offset);
	offset = mLevelManager.getOffset(mMaps.primaryMap);

	mLevelManager.buildExit(mMaps.exit, offset);

	setCameraBoundaries();

	DebugPrint(Log, "\n Loading Effect pool\n");

	mEffectPool.load();

	DebugPrint(Log, "\n Loading Characters\n");

	mActors.load();

	DebugPrint(Log, "\n--- Environment Load Complete---\n\n");

	std::vector<SpawnData> data = mSpawner.getspawnList(mMaps.primaryMap, mLevelManager.level());

	int a = 4;
}


void Environment::nextLevel()
{
	// wipe enemies

	mLevelManager.incrementLevel();
	mLevelManager.swapEntranceExit(mMaps.entrance, mMaps.exit);

	VectorF offset = mLevelManager.getOffset(mMaps.entrance);
	mLevelManager.buildPrimary(mMaps.primaryMap, offset);

	offset = mLevelManager.getOffset(mMaps.primaryMap);
	mLevelManager.buildExit(mMaps.exit, offset);

	setCameraBoundaries();

	IncrementLevelEvent event;
	notify(event);

	// spawn new enemies

	std::vector<SpawnData> data = mSpawner.getspawnList(mMaps.primaryMap, mLevelManager.level());

	int a = 4;
}

void Environment::handleInput()
{
	mActors.handleInput();
}

void Environment::exit()
{
	mActors.exit();
	restart();
}

void Environment::fastUpdate(float dt)
{
	mActors.fastUpdate(dt);
}

void Environment::slowUpdate(float dt)
{
	mMaps.entrance->slowUpdate(dt);
	mMaps.primaryMap->slowUpdate(dt);
	mMaps.exit->slowUpdate(dt);

	mActors.slowUpdate(dt);
}


void Environment::renderFloor()
{
	mMaps.entrance->renderFloor();
	mMaps.primaryMap->renderFloor();
	mMaps.exit->renderFloor();
}
void Environment::renderBottomLayer()
{
	mMaps.entrance->renderLowerLayer();
	mMaps.primaryMap->renderLowerLayer();
	mMaps.exit->renderLowerLayer();

	mActors.render();
}
void Environment::renderTopLayer()
{
	mMaps.entrance->renderUpperLayer();
	mMaps.primaryMap->renderUpperLayer();
	mMaps.exit->renderUpperLayer();
}


VectorF Environment::size() const
{
	return mMaps.primaryMap->size();
}


Map* Environment::map(VectorF position) const
{
	if (position.x < mMaps.primaryMap->getFirstRect().LeftPoint())
		return mMaps.entrance;
	else if (position.x < mMaps.primaryMap->getLastRect().RightPoint())
		return mMaps.primaryMap;
	else
		return mMaps.exit;
}


void Environment::setCameraBoundaries()
{
	float xLeft = mMaps.entrance->getFirstRect().LeftPoint();
	float xRight = mMaps.exit->getLastRect().RightPoint();
	RectF boundaries(xLeft, 0.0f, xRight, mMaps.primaryMap->size().y);

	Camera::Get()->setMapBoundaries(boundaries);
}


bool Environment::canClosePreviousLevel(VectorF playerPosition) const
{
	float buffer = 100.0f;
	float x = mMaps.primaryMap->getLastRect().RightPoint() + buffer;
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
	float xOffset = mMaps.entrance->getLastRect().RightPoint();

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

	mMaps.primaryMap = new Map(primaryMapSize, tileSize);


	Vector2D<int> corridorMapSize;
	corridorMapSize.x = (int)((Camera::Get()->size().x / mMaps.primaryMap->tileSize().x) * 1.5f);
	corridorMapSize.y = primaryMapSize.y;

	mMaps.entrance = new Map(corridorMapSize, tileSize);
	mMaps.exit = new Map(corridorMapSize, tileSize);
}