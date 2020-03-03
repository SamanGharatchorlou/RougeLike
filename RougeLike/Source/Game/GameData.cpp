#include "pch.h"
#include "GameData.h"

// GameData
#include "Graphics/Renderer.h"
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
	textureManager->init(renderer);

	cursor = new Cursor();
	cursor->setSize(25.0f, 25.0f);

	// Must be before UIManager
	camera = new Camera();
	camera->setViewport(1024, 768);

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

	// Renderer
	// TODO: replace with singleton
	renderManager = new RenderManager(this);

	// Score Manager
	scoreManager = new ScoreManager;
	scoreManager->addObserver(uiManager);

	// Player
	playerManager = new PlayerManager(this);
	playerManager->addObserver(uiManager);

	// Enemies
	enemies = new EnemyManager(this);
	enemies->addObserver(scoreManager);
	enemies->addObserver(playerManager);
}


void GameData::free()
{
	// destroy renderer
	SDL_DestroyRenderer(renderer);
	renderer = nullptr;

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
	delete camera;
	delete cursor;

	delete playerManager;
	delete enemies;

	DebugPrint(Log, "All game data has been deleted\n");
}
