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
#include "Actors/ActorManager.h"
#include "Objects/Effects/EffectPool.h"
#include "Actors/Enemies/EnemyManager.h"

#include "Map/Environment.h"
#include "Game/Camera.h"


// GameInfo
#include "Map/Map.h"


void GameData::load()
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
	environment = new Environment(textureManager);

	// Audio
	audioManager = new AudioManager;
	audioManager->init();

	// UI
	uiManager = new UIManager(this);
	uiManager->init();

	// Rendering
	renderManager = new RenderManager;

	// Score Manager
	scoreManager = new ScoreManager;

	// Collision Trackers
	collisionManager = new CollisionManager;

	// Effect Pool, must be before actors
	effectPool = new EffectPool(this);

	// Actors
	actors = new ActorManager(this);

	// Must be done AFTER everything has been new'd
	setupObservers();
}


void GameData::setupObservers()
{
	// Update the current map level
	environment->addObserver(scoreManager);

	// Update the UI with all scores
	scoreManager->addObserver(uiManager);

	// Update the UI with the players hp and the stats attack, defence etc.
	actors->addObserver(actors);
	actors->addObserver(uiManager);
	actors->addObserver(Camera::Get()->getShake());
	actors->addObserver(scoreManager);
}


void GameData::free()
{
	delete actors;

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
