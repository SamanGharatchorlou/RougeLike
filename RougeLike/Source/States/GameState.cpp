#include "pch.h"
#include "GameState.h"

#include "Graphics/Renderer.h"
#include "Game/Data/GameData.h"
#include "Game/Environment.h"

#include "Audio/AudioManager.h"
#include "Graphics/RenderManager.h"
#include "Managers/ScoreManager.h"
#include "Actors/ActorManager.h"
#include "Actors/Player/Player.h"

#include "Map/Map.h"
#include "Game/Camera/Camera.h"


GameState::GameState(GameData* gameData) : mGameData(gameData) { }


void GameState::init()
{
	Map* map = mGameData->environment->map(VectorF(0, 0));
	VectorF playerPosition = map->randomFloorTile(50)->rect().Center();

	mGameData->environment->actors()->player()->setPosition(playerPosition);

	initCamera();
	initRendering();

	// Start Audio
	AudioManager* audio = AudioManager::Get();
	audio->fadeIn("Game", nullptr, 1500);
	audio->setSource(mGameData->environment->actors()->player()->get(), Camera::Get()->size().x);
}


void GameState::handleInput()
{
	mGameData->environment->handleInput(mGameData->inputManager);
}


void GameState::fastUpdate(float dt)
{
	Camera::Get()->fastUpdate(dt);
	mGameData->environment->fastUpdate(dt);
}


void GameState::slowUpdate(float dt)
{
	Camera::Get()->slowUpdate(dt);
	mGameData->environment->slowUpdate(dt);
	mGameData->scoreManager->slowUpdate();
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
	mGameData->environment->resume();

	AudioManager* audio = AudioManager::Get();
	audio->fadeIn("Game", nullptr, 750);
}

void GameState::pause()
{
	mGameData->environment->pause();

	AudioManager* audio = AudioManager::Get();
	audio->fadeOut("Game", nullptr, 150);
}


void GameState::exit()
{
	mGameData->environment->clear();
	mGameData->scoreManager->reset();

	AudioManager* audio = AudioManager::Get();
	audio->fadeOut("Game", nullptr, 150);
}


// --- Private Functions --- //

void GameState::initCamera()
{
	Camera* camera = Camera::Get();

	VectorF cameraPosition = VectorF(0.0f, 0.0f);
	camera->setPosition(cameraPosition);

	// TODO: fix these values
	camera->initShakeyCam(140.0f, 100.0f);

	RectF* playerRect = &mGameData->environment->actors()->player()->get()->rectRef();
	camera->follow(playerRect);
}


void GameState::initRendering()
{
	mGameData->renderManager->Set(mGameData->environment);
	mGameData->renderManager->Set(mGameData->uiManager);
}
