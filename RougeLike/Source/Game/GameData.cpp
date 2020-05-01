#include "pch.h"
#include "GameData.h"

// GameData
#include "System/Window.h"
#include "Graphics/TextureManager.h"
#include "Audio/AudioManager.h"
#include "Input/inputManager.h"
#include "Graphics/RenderManager.h"
#include "UI/UIManager.h"
#include "Managers/ScoreManager.h"
#include "Collisions/CollisionManager.h"
#include "Characters/Player/PlayerManager.h"
#include "Characters/Enemies/EnemyManager.h"

#include "Map/Environment.h"
#include "Game/Camera.h"
#include "Game/Cursor.h"

// GameInfo
#include "Map/Map.h"


void GameData::init()
{

	// Texture Manager: must be before cursor
	textureManager = new TextureManager;
	textureManager->init();

	// Cursor
	cursor = new Cursor;
	cursor->setSize(25.0f, 25.0f);

	// Set camera before UIManager
	Camera::Get()->setViewport(window->size());

	// Map Level
	environment = new Environment;

	// Audio
	audioManager = new AudioManager;
	audioManager->init();

	// Input
	inputManager = new InputManager(cursor);
	inputManager->init();

	// UI
	uiManager = new UIManager(this);
	uiManager->init();

	// Rendering
	renderManager = new RenderManager(this);

	// Score Manager
	scoreManager = new ScoreManager;

	// Player
	playerManager = new PlayerManager(this);

	// TEMP
	playerManager->addObserver(Camera::Get()->getShake());

	// Enemies
	enemies = new EnemyManager(this);

	// Collision Trackers
	collisionManager = new CollisionManager;


	// Setup gameinfo
	GameInfo::Get()->map(environment);

	// Must be done AFTER everything has been new'd
	setupObservers();
}

// TODO: centralise some of these systems?
void GameData::setupObservers()
{
	// Update the current map level
	environment->addObserver(scoreManager);

	// Update the UI with all scores
	scoreManager->addObserver(uiManager);
	// Update the UI with the players hp and the stats attack, defence etc.
	playerManager->addObserver(uiManager);

	// Update enemy paths
	playerManager->addObserver(enemies);


	// Update the score
	enemies->addObserver(scoreManager);
	// Player gains exp
	enemies->addObserver(playerManager);
}


void GameData::free()
{
	// destory window (created in gamecontroller)
	delete window;

	// managers
	// ERROR: when there are no textures or audio files in the map gives error... why?
	delete textureManager;
	delete audioManager;
	delete inputManager;
	delete renderManager;
	delete uiManager;
	delete scoreManager;
	delete collisionManager;

	delete environment;
	delete cursor;

	delete playerManager;
	delete enemies;

	DebugPrint(Log, "All game data has been deleted\n");
}



// -- Game Info -- //
const VectorF GameInfo::mapDimentions() const
{
	return mMap->size();
}

bool GameInfo::isWall(VectorF position) const
{
	const MapTile* tile = mMap->map(position)->tile(position);
	return tile ? tile->hasCollisionType(MapTile::Wall) : false;
}