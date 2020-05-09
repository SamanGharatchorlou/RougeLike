#include "pch.h"
#include "GameState.h"

#include "PauseState.h"

#include "Managers/GameController.h"
#include "Graphics/TextureManager.h"
#include "Audio/AudioManager.h"
#include "Graphics/RenderManager.h"
#include "Input/InputManager.h"
#include "UI/UIManager.h"
#include "Managers/ScoreManager.h"
#include "Collisions/CollisionManager.h"

#include "Map/Environment.h"
#include "Game/Cursor.h"
#include "Game/Camera.h"

#include "Objects/Player/PlayerManager.h"
#include "Objects/Enemies/EnemyManager.h"

// TEMp
#include "Items/Spawner.h"
#include "Objects/Player/Player.h"


GameState::GameState(GameData* gameData, GameController* gameController) : 
	mGameData(gameData)
	, mGameController(gameController)
	, mCollectables(gameData)
{
	// random seed
	srand((unsigned int)time(NULL));
}


void GameState::init()
{
	mGameData->uiManager->selectScreen(Screen::CharacterSelection);

	// Entrance width
	initMap();

	// Camera
	initCamera();

	// UI
	initUI();

	// Player
	initPlayer();

	// Enemies
	initEnemies();

	// Start Audio
	//mGameData->audioManager->playMusic("Ludumdum");

	// Set cursor
	mGameData->uiManager->setCursorTexture(mGameData->textureManager->getTexture("GameCursor"));

	// Rendering
	initRendering();

	FileManager::Get()->allFilesInFolder(FileManager::Audio_Sound);

	mCollectables.spawnRandomItem(Collectables::MeleeWeapon);
}


void GameState::handleInput()
{
	if (mGameData->inputManager->isPressed(Button::Esc))
	{
		mGameController->quitGame();
	}
	
	if (mGameData->inputManager->isPressed(Button::Pause))
	{
		mGameController->getStateMachine()->addState(new PauseState(mGameData, mGameController));
	}

	mGameData->playerManager->handleInput();
}


void GameState::fastUpdate(float dt)
{
	mGameData->playerManager->fastUpdate(dt);
	mGameData->enemies->fastUpdate(dt);

	Camera::Get()->fastUpdate(dt);

	mGameData->collisionManager->processCollisions();
}


void GameState::slowUpdate(float dt)
{
	mGameData->playerManager->slowUpdate(dt);
	mGameData->enemies->slowUpdate(dt);

	mGameData->scoreManager->slowUpdate();

	mCollectables.slowUpdate(dt);

	Camera::Get()->slowUpdate(dt);

	// End current level, close old level exit, open new level entrance
	if (mGameData->environment->generateNextLevel(mGameData->playerManager->rect()->TopLeft()))
	{
		nextLevel();
	}

	// Close off new level entrance, open exit
	if (mGameData->environment->closeEntrance(mGameData->playerManager->rect()->TopLeft()))
	{
		mGameData->environment->closeLevelEntrace();
		Camera::Get()->setMapBoundaries(mGameData->environment->boundaries());
	}

	// End of slow frame
	mGameData->collisionManager->resetColliders();
}


void GameState::render()
{
	SDL_Renderer* renderer = Renderer::Get()->sdlRenderer();

	// clear screen
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(renderer);

	mGameData->renderManager->render();

	// update window surface
	SDL_RenderPresent(renderer);
}


void GameState::resume()
{
	mGameData->uiManager->setCursorTexture(mGameData->textureManager->getTexture("GameCursor"));
}


void GameState::exit()
{
	mGameData->enemies->clear();
	mGameData->playerManager->reset();
	mGameData->environment->restart();
	mGameData->scoreManager->reset();
	mGameData->collisionManager->clearColliders();
}


/// --- Private Functions --- ///

void GameState::initUI()
{
	mGameData->uiManager->setCursorTexture(mGameData->textureManager->getTexture("GameCursor"));
	mGameData->uiManager->selectScreen(Screen::Game);
}

void GameState::initCamera()
{
	VectorF cameraPosition = VectorF(0.0f, mGameData->environment->size().y / 2.0f);
	Camera::Get()->setPosition(cameraPosition);
	Camera::Get()->setMapBoundaries(mGameData->environment->boundaries());
}

void GameState::initMap()
{
	float entraceOffset = Camera::Get()->size().x * 1.5f;
	mGameData->environment->init();
}

void GameState::initPlayer()
{
	mGameData->playerManager->initCollisions();

	VectorF playerPosition = VectorF(Camera::Get()->getCenter().x, mGameData->environment->size().y / 2.0f);
	mGameData->playerManager->rect()->SetLeftCenter(playerPosition);

	// Camera
	Camera::Get()->follow(mGameData->playerManager->rect());
}

void GameState::initEnemies()
{
	EnemyManager* enemies = mGameData->enemies;
	enemies->setTarget(mGameData->playerManager->rect());
	enemies->spawnLevel();

	// init AI pathing
	enemies->generatePathMap();
}

void GameState::initRendering()
{
	mGameData->renderManager->Set(mGameData->environment);
	mGameData->renderManager->Set(mGameData->playerManager);
	mGameData->renderManager->Set(mGameData->enemies);
	mGameData->renderManager->Set(mGameData->uiManager);
	mGameData->renderManager->Set(&mCollectables);
}


void GameState::nextLevel()
{
	mGameData->environment->nextLevel();
	Camera::Get()->setMapBoundaries(mGameData->environment->boundaries());

	// end level
	mGameData->enemies->clearAllEnemies();
	mGameData->enemies->spawnLevel();

	mCollectables.spawnRandomItem(Collectables::MeleeWeapon);
}