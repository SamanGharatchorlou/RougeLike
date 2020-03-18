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
#include "Characters/Player/PlayerManager.h"
#include "Characters/Enemies/EnemyManager.h"

#include "Map/MapLevel.h"
#include "Game/Camera.h"
#include "Game/Cursor.h"

// GameInfo
#include "Map/Map.h"


void GameData::init()
{
	// Texture Manager: must be before cursor
	textureManager = new TextureManager();
	textureManager->init();

	// Cursor
	cursor = new Cursor();
	cursor->setSize(25.0f, 25.0f);

	// Set camera before UIManager
	Camera::Get()->setViewport(window->size());

	// Map Level
	level = new MapLevel();

	// Audio
	audioManager = new AudioManager();
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
	scoreManager->addObserver(uiManager);

	// Player
	playerManager = new PlayerManager(this);
	playerManager->addObserver(uiManager);

	// TEMP
	playerManager->addObserver(Camera::Get()->getShake());

	// Enemies
	enemies = new EnemyManager(this);
	enemies->addObserver(scoreManager);
	enemies->addObserver(playerManager);


	// Setup gameinfo
	GameInfo::Get()->map(level);
}


void GameData::free()
{
	// destory window
	delete window;

	// managers
	// ERROR: when there are no textures or audio files in the map gives error... why?
	delete textureManager;
	delete audioManager;
	delete inputManager;
	delete renderManager;
	delete uiManager;
	delete scoreManager;

	delete level;
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
	const MapTile* tile = mMap->map()->getTile(position);
	return tile ? tile->hasCollisionType(MapTile::Wall) : false;
}