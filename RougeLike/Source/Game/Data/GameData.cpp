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

#include "Game/Environment.h"
#include "Game/Camera/Camera.h"


void GameData::setWindow(Window* newWindow)
{
	window = newWindow;
}


void GameData::load()
{
	// Texture Manager
	TextureManager::Get()->load();

	// Set camera before UIManager
	Camera::Get()->setViewport(window->size());

	// Input
	inputManager = new InputManager;
	inputManager->init();
	inputManager->setCursorSize(VectorF(25.0f, 25.0f));

	// Audio
	AudioManager::Get()->load();

	// UI
	uiManager = new UIManager;
	uiManager->setupScreens();
	uiManager->initCursor(inputManager->getCursor());

	// Rendering
	renderManager = new RenderManager;

	// Score Manager
	scoreManager = new ScoreManager;

	// Collision Trackers
	collisionManager = new CollisionManager;
	collisionManager->init();

	// Map Level
	environment = new Environment;
	environment->init(this);
	environment->load();

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
	environment->actors()->addObserver(environment->actors());
	environment->actors()->addObserver(uiManager);
	environment->actors()->addObserver(Camera::Get()->getShake());
	environment->actors()->addObserver(scoreManager);
	environment->actors()->addObserver(renderManager);
}


void GameData::free()
{
	delete collisionManager;
	delete environment;

	delete scoreManager;
	delete uiManager;

	delete inputManager;

	AudioManager::Get()->unload();
	TextureManager::Get()->unload();

	delete renderManager;

	delete window;

	DebugPrint(Log, "All game data has been deleted\n");
}
