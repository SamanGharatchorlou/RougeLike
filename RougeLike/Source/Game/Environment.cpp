#include "pch.h"
#include "Environment.h"

#include "Game/Data/GameData.h"
#include "Game/Camera/Camera.h"
#include "Input/InputManager.h"

#include "Map/MapBuilding/MapGenerator.h"
#include "Map/Map.h"

#include <thread>


Environment::~Environment()
{
	clear();
}


void Environment::clear()
{
	mActors.clear();

	mLevelManager.clear();
	mEffectPool.freeAll();

	mMapSize = Vector2D<int>();

	Camera::Get()->clear();
}


void Environment::init(GameData* gameData)
{
	mLevelManager.init(this);
	mActors.init(gameData);
}


void Environment::load()
{
	DebugPrint(Log, "\n--- Loading Environment ---\n\n");

	DebugPrint(Log, "\n Loading pools\n");

	std::vector<EffectType> types;
	types.reserve((int)EffectType::Count);
	for (EffectType type = EffectType::None + 1; type < EffectType::Count; type = type + 1)
	{
		types.push_back(type);
	}
	mEffectPool.load(types, 25);

	// Some of these types we will be using a lot, so add more now
	types.clear();
	for (EffectType type = EffectType::Damage; type < EffectType::Count; type = type + 1)
	{
		types.push_back(type);
	}
	mEffectPool.load(types, 50);
	
	mActors.loadPools();

	DebugPrint(Log, "\n Loading Maps\n");

	mLevelManager.load();

	setCameraBoundaries();

	DebugPrint(Log, "\n--- Environment Load Complete---\n\n");
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
}


void Environment::renderFloor()
{
	mLevelManager.renderFloor();
}


void Environment::renderBottomLayer()
{
	mLevelManager.renderLowDepth();
	mActors.render();
}

void Environment::renderTopLayer()
{
	mLevelManager.renderHighDepth();
}


void Environment::fillRenderQueue()
{
    mLevelManager.fillRenderQueue_Floor();
    mLevelManager.fillRenderQueue_LowDepth();
    mActors.fillRenderQueue();
    mLevelManager.fillRenderQueue_HighDepth();
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
