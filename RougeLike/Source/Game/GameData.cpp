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

#include "Map/Map.h"
#include "Game/Camera.h"
#include "Game/Cursor.h"


void GameData::init()
{
	// Must be before cursor
	textureManager = new TextureManager();
	textureManager->init();

	cursor = new Cursor();
	cursor->setSize(25.0f, 25.0f);

	// Set camera before UIManager
	Camera::Get()->setViewport(1024, 768);

	map = new Map(this);

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

	delete map;
	delete cursor;

	delete playerManager;
	delete enemies;

	DebugPrint(Log, "All game data has been deleted\n");
}
