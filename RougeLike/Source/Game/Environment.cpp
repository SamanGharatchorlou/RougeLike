#include "pch.h"
#include "Environment.h"

#include "Game/Data/GameData.h"
#include "Game/Camera/Camera.h"
#include "Input/InputManager.h"

#include "Map/MapBuilding/MapGenerator.h"
#include "Map/Map.h"


Environment::Environment(GameData* gameData) : 
	mLevelManager(gameData->collisionManager, &mEffectPool), 
	mActors(gameData), 
	mCollectables(gameData)
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

	DebugPrint(Log, "\n Loading Effect pool\n");

	std::vector<EffectType> types;
	for (EffectType type = EffectType::None + 1; type < EffectType::Count; type = type + 1)
	{
		types.push_back(type);
	}
	mEffectPool.load(types, 25);

	DebugPrint(Log, "\n Loading Maps\n");

	BasicString fileName = "Level0";
	BasicString path = FileManager::Get()->findFile(FileManager::Config_Map, fileName);
	XMLParser parser(path);

	mLevelManager.load(parser);

	setCameraBoundaries();

	DebugPrint(Log, "\n Loading Characters\n");

	mActors.load(parser);

	XMLNode enemySpawnNode = parser.rootChild("Enemies");
	mActors.spawnEnemies(enemySpawnNode, mLevelManager.map(MapType::Dungeon));

	DebugPrint(Log, "\n Loading Collectables\n");

	mCollectables.load();
	XMLNode collectablesSpawnNode = parser.rootChild("Collectables");
	mCollectables.spawn(collectablesSpawnNode, mLevelManager.map(MapType::Dungeon));

	DebugPrint(Log, "\n--- Environment Load Complete---\n\n");
}


void Environment::nextLevel()
{
	// TODO: wipe enemies / add some exit level thing
	mLevelManager.incrementLevel();

	BasicString fileName = "Level" + BasicString(mLevelManager.level());
	BasicString path = FileManager::Get()->findFile(FileManager::Config_Map, fileName);
	XMLParser parser(path);

	//mLevelManager.buildLevel(parser);

	setCameraBoundaries();

	IncrementLevelEvent event;
	notify(event);

	// spawn new enemies
	XMLNode enemySpawnNode = parser.rootChild("Enemies");
	//mActors.spawnEnemies(enemySpawnNode, mLevelManager.primaryMap());

	// spawn collectables
	XMLNode collectablesSpawnNode = parser.rootChild("Collectables");
	//mCollectables.spawn(collectablesSpawnNode, mLevelManager.primaryMap());
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
	mLevelManager.renderHighDepth();
}


void Environment::setCameraBoundaries()
{
	VectorF topLeft = mLevelManager.first()->getFirstRect().TopLeft();
	VectorF bottomRight = mLevelManager.last()->getBottomLastRect().BotRight();

	RectF boundaries(topLeft.x, topLeft.y, bottomRight.x, bottomRight.y);
	Camera::Get()->setMapBoundaries(boundaries);
}


bool Environment::canClosePreviousLevel(VectorF playerPosition) const
{
	//float buffer = 100.0f;
	//float x = mLevelManager.primaryMap()->getLastRect().RightPoint() + buffer;
	//float y = playerPosition.y;

	//if (playerPosition.x > x)
	//	return !Camera::Get()->inView(VectorF(x, y));
	//else
		return false;
}



// --- Private Functions --- //
VectorF Environment::toWorldCoords(VectorF cameraCoords)
{
	VectorF cameraPosition = Camera::Get()->rect().TopLeft();
	return cameraCoords + cameraPosition;
}