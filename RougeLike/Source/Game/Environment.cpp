#include "pch.h"
#include "Environment.h"

#include "Game/Data/GameData.h"
#include "Game/Camera/Camera.h"
#include "Input/InputManager.h"

#include "Map/MapBuilding/MapGenerator.h"
#include "Map/Map.h"



void Environment::clear()
{
	mActors.clear();
	mCollectables.clear();

	mLevelManager.clear();
	mEffectPool.freeAll();

	mMapSize = Vector2D<int>();
}

void Environment::init(GameData* gameData)
{
	mLevelManager.init(gameData->collisionManager, &mEffectPool);
	mActors.init(gameData);

	mCollectables.init(gameData->collisionManager, mActors.player());
}

// TODO load all pools first
void Environment::load()
{
	DebugPrint(Log, "\n--- Loading Environment ---\n\n");

	DebugPrint(Log, "\n Loading pools\n");

	std::vector<EffectType> types;
	for (EffectType type = EffectType::None + 1; type < EffectType::Count; type = type + 1)
	{
		types.push_back(type);
	}
	mEffectPool.load(types, 25);
	
	mActors.loadPools();

	DebugPrint(Log, "\n Loading Maps\n");

	mLevelManager.load();

	setCameraBoundaries();

	DebugPrint(Log, "\n Loading Characters\n");

	DebugPrint(Log, "\n Loading Collectables\n");

	mCollectables.load();

	// TODO: FIX remove tis
	BasicString fileName = "Level0";
	BasicString path = FileManager::Get()->findFile(FileManager::Config_Map, fileName);
	XMLParser parser(path);
	XMLNode collectablesSpawnNode = parser.rootChild("Collectables");
	mCollectables.spawn(collectablesSpawnNode, mLevelManager.firstMap(MapType::Dungeon));

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

	// spawn collectables
	XMLNode collectablesSpawnNode = parser.rootChild("Collectables");
}

void Environment::handleInput(const InputManager* input)
{
	mActors.handleInput(input);
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
	VectorF topLeft = mLevelManager.firstMap(MapType::None)->getFirstRect().TopLeft();
	VectorF bottomRight = mLevelManager.lastMap(MapType::None)->getBottomLastRect().BotRight();

	RectF boundaries(topLeft.x, topLeft.y, bottomRight.x, bottomRight.y);
	Camera::Get()->setMapBoundaries(boundaries);
}


// --- Private Functions --- //
VectorF Environment::toWorldCoords(VectorF cameraCoords)
{
	VectorF cameraPosition = Camera::Get()->rect().TopLeft();
	return cameraCoords + cameraPosition;
}