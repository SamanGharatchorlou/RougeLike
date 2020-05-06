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
#include "Objects/Player/PlayerManager.h"
#include "Objects/Enemies/EnemyManager.h"

#include "Map/Environment.h"
#include "Game/Camera.h"

// GameInfo
#include "Map/Map.h"


void GameData::init()
{
	// Texture Manager
	textureManager = new TextureManager;
	textureManager->init();

	// Set camera before UIManager
	Camera::Get()->setViewport(window->size());

	// Input
	inputManager = new InputManager;
	inputManager->init();
	inputManager->setCursorSize(VectorF(25.0f, 25.0f));

	// Map Level
	environment = new Environment;

	// Audio
	audioManager = new AudioManager;
	audioManager->init();

	// UI
	uiManager = new UIManager(this);
	uiManager->init();

	// Rendering
	renderManager = new RenderManager(this);

	// Score Manager
	scoreManager = new ScoreManager;

	// Collision Trackers
	collisionManager = new CollisionManager;

	// Player
	playerManager = new PlayerManager(this);

	// Enemies
	enemies = new EnemyManager(this);
	enemies->init();

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
	playerManager->addObserver(Camera::Get()->getShake());

	// Update the score
	enemies->addObserver(scoreManager);
	// Player gains exp
	enemies->addObserver(playerManager);
}


void GameData::free()
{
	delete enemies;
	delete playerManager;

	delete collisionManager;
	delete environment;

	delete scoreManager;
	delete uiManager;

	delete inputManager;
	delete textureManager;
	delete audioManager;
	delete renderManager;

	delete window;

	DebugPrint(Log, "All game data has been deleted\n");
}
