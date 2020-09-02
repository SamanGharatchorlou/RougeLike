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
#include "Actors/ActorManager.h"

#include "Game/Environment.h"
#include "Game/Camera/Camera.h"


void GameData::setWindow(Window* newWindow)
{
	window = newWindow;
}

void GameData::init(GameController* gameController)
{
	// Set camera before UIManager
	Camera::Get()->setViewport(window->size().toFloat());

	// Input
	inputManager = new InputManager;

	// UI
	uiManager = new UIManager;
	uiManager->init(gameController);

	// Rendering
	renderManager = new RenderManager;

	// Score Manager
	scoreManager = new ScoreManager;

	// Map Level
	environment = new Environment;
}


void GameData::preLoad()
{
	TextureManager::Get()->preLoad();

	uiManager->preLoad();
	uiManager->initCursor(inputManager->getCursor());
}

void GameData::load()
{
	// Texture Manager
	TextureManager::Get()->load();

	// Set camera before UIManager
	Camera::Get()->setViewport(window->size().toFloat());

	// Input
	inputManager->init();
	inputManager->setCursorSize(VectorF(25.0f, 25.0f));

	// Audio
	AudioManager::Get()->load();

	// UI
	//uiManager->setupScreens();
	uiManager->load();
	uiManager->initCursor(inputManager->getCursor());

	// Map Level
	environment->init(this);
	environment->load();
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

	environment->levelManager()->addObserver(environment->actors());
}


void GameData::free()
{
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
