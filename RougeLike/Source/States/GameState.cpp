#include "pch.h"
#include "GameState.h"

#include "PauseState.h"

#include "Game/GameController.h"
#include "Graphics/TextureManager.h"
#include "Audio/AudioManager.h"
#include "Graphics/RenderManager.h"
#include "Input/InputManager.h"
#include "UI/UIManager.h"
#include "Managers/ScoreManager.h"
#include "Collisions/CollisionManager.h"

#include "Objects/Pools/EffectPool.h"

#include "Game/Environment.h"
#include "Input/Cursor.h"
#include "Game/Camera/Camera.h"

#include "Actors/ActorManager.h"
#include "Actors/Player/Player.h"
#include "Actors/Enemies/EnemyManager.h"

// temp
#include "Items/Collectables/Collectable.h"
#include "Map/Map.h"

GameState::GameState(GameData* gameData, GameController* gameController) : 
	mGameData(gameData)
	, mGameController(gameController)
{
}


void GameState::init()
{
	Map* map = mGameData->environment->map(VectorF(0, 0));
	VectorF playerPosition = map->randomFloorTile(50)->rect().Center();

	mGameData->environment->actors()->player()->setPosition(playerPosition);

	initCamera();
	initUI();
	initRendering();


	// Start Audio
	AudioManager* audio = AudioManager::Get();
	audio->playMusic("Ludumdum");
	audio->setMusicVolume(0.0f);
	audio->setSoundVolume(0.0f);
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

	mGameData->environment->handleInput(mGameData->inputManager);
}


void GameState::fastUpdate(float dt)
{
	Camera::Get()->fastUpdate(dt);

	mGameData->collisionManager->fastUpdate();

	mGameData->environment->fastUpdate(dt);
}


void GameState::slowUpdate(float dt)
{
	mGameData->environment->slowUpdate(dt);

	mGameData->scoreManager->slowUpdate();

	Camera::Get()->slowUpdate(dt);

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
	mGameData->uiManager->setCursorTexture(TextureManager::Get()->getTexture("GameCursor", FileManager::Image_UI));
}


void GameState::exit()
{
	mGameData->environment->clear();
	mGameData->scoreManager->reset();
	mGameData->collisionManager->clearColliders();
}


// --- Private Functions --- //

void GameState::initUI()
{
	mGameData->uiManager->setCursorTexture(TextureManager::Get()->getTexture("GameCursor", FileManager::Image_UI));
	mGameData->uiManager->pushScreen(ScreenType::Game);
}

void GameState::initCamera()
{
	Camera* camera = Camera::Get();

	VectorF cameraPosition = VectorF(0.0f, 0.0f);
	camera->setPosition(cameraPosition);

	// TODO: fix these values
	camera->initShakeyCam(150.0f, 100.0f, 50.0f);

	RectF* playerRect = &mGameData->environment->actors()->player()->get()->rectRef();
	camera->follow(playerRect);
}


void GameState::initRendering()
{
	mGameData->renderManager->Set(mGameData->environment);
	mGameData->renderManager->Set(mGameData->uiManager);
}
