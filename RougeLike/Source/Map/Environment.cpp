#include "pch.h"
#include "Environment.h"

#include "Game/GameData.h"
#include "Game/Camera.h"
#include "Input/InputManager.h"
#include "MapBuilding/MapGenerator.h"
#include "Map.h"

#include "TrapManager.h"


Environment::Environment(GameData* gameData) : 
	mLevelManager(gameData->textureManager), mActors(gameData), mCollectables(gameData)
{
};



void Environment::restart()
{
	DebugPrint(Log, "Environment restart unimplemented\n");
	//delete mMaps.entrance;
	//delete mMaps.primaryMap;
	//delete mMaps.exit;

	//createNewMaps();
}

void Environment::init()
{
	mActors.init(this);
	mCollectables.setCollector(mActors.player());
}


void Environment::load()
{
	DebugPrint(Log, "\n--- Loading Environment ---\n\n");

	DebugPrint(Log, "\n Loading Maps\n");

	BasicString fileName = "Level0";
	BasicString path = FileManager::Get()->findFile(FileManager::Config_Map, fileName);
	XMLParser parser(path);

	mLevelManager.load(parser);

	setCameraBoundaries();

	DebugPrint(Log, "\n Loading Effect pool\n");

	mEffectPool.load();

	DebugPrint(Log, "\n Loading Characters\n");

	mActors.load(parser, mLevelManager.primaryMap());

	XMLNode enemySpawnNode = parser.rootChild("Enemies");
	mActors.spawnEnemies(enemySpawnNode, mLevelManager.primaryMap());

	DebugPrint(Log, "\n Loading Collectables\n");

	mCollectables.load();
	XMLNode collectablesSpawnNode = parser.rootChild("Collectables");
	mCollectables.spawn(collectablesSpawnNode, mLevelManager.primaryMap());

	DebugPrint(Log, "\n--- Environment Load Complete---\n\n");
}


void Environment::nextLevel()
{
	// wipe enemies

	// TODO: read the level xml parser here
	// use this parser and pass it into every object that needs it for setup
	// e.g. level mgr and spawners

	mLevelManager.incrementLevel();

	char character = (char)mLevelManager.level();

	BasicString fileName = "Level" + BasicString(&character);
	BasicString path = FileManager::Get()->findFile(FileManager::Config_Map, fileName);
	XMLParser parser(path);

	mLevelManager.buildLevel(parser);

	setCameraBoundaries();

	IncrementLevelEvent event;
	notify(event);

	// spawn new enemies
	XMLNode enemySpawnNode = parser.rootChild("Enemies");
	mActors.spawnEnemies(enemySpawnNode, mLevelManager.primaryMap());

	// spawn collectables
	XMLNode collectablesSpawnNode = parser.rootChild("Collectables");
	mCollectables.spawn(collectablesSpawnNode, mLevelManager.primaryMap());
}

void Environment::handleInput(const InputManager* input)
{
	mActors.handleInput(input);
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
	mLevelManager.slowUpdate(dt);
	mActors.slowUpdate(dt);
	mCollectables.slowUpdate(dt);
}


void Environment::renderBottomLayer()
{
	mLevelManager.renderLowDepth();
	mCollectables.render();
	mActors.render();
}

void Environment::renderTopLayer()
{
	mLevelManager.renderHeighDepth();
}


void Environment::setCameraBoundaries()
{
	float xLeft = mLevelManager.entrance()->getFirstRect().LeftPoint();
	float xRight = mLevelManager.exit()->getLastRect().RightPoint();
	RectF boundaries(xLeft, 0.0f, xRight, mLevelManager.primaryMap()->size().y);

	Camera::Get()->setMapBoundaries(boundaries);
}


bool Environment::canClosePreviousLevel(VectorF playerPosition) const
{
	float buffer = 100.0f;
	float x = mLevelManager.primaryMap()->getLastRect().RightPoint() + buffer;
	float y = playerPosition.y;

	if (playerPosition.x > x)
		return !Camera::Get()->inView(VectorF(x, y));
	else
		return false;
}



// --- Private Functions --- //
VectorF Environment::toWorldCoords(VectorF cameraCoords)
{
	float xOffset = mLevelManager.entrance()->getLastRect().RightPoint();

	VectorF cameraPosition = Camera::Get()->rect().TopLeft();

	float xDiff = cameraPosition.x - xOffset;

	return cameraCoords + cameraPosition;
}