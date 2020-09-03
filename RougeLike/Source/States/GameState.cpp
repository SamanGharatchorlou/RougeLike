#include "pch.h"
#include "GameState.h"

// TODO: clean up these includes
#include "Game/Data/GameData.h"

#include "Audio/AudioManager.h"
#include "Graphics/RenderManager.h"
#include "Input/InputManager.h"
#include "Managers/ScoreManager.h"
#include "UI/UIManager.h"

#include "Game/Environment.h"
#include "Map/Map.h"
#include "Game/Camera/Camera.h"

#include "Actors/ActorManager.h"
#include "Actors/Player/Player.h"
#include "Actors/Enemies/EnemyManager.h"


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
	audio->playMusic("Ludumdum");
	audio->setMusicVolume(0.0f);
	audio->setSoundVolume(25.0f);
/*
	mGameData->uiManager->controller()->openPopup("Attack");
	mGameData->uiManager->controller()->openPopup("Movement");*/

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
}
void GameState::pause()
{
	mGameData->environment->pause();
}


void GameState::exit()
{
	mGameData->environment->clear();
	mGameData->scoreManager->reset();
}


// --- Private Functions --- //

void GameState::initCamera()
{
	Camera* camera = Camera::Get();

	VectorF cameraPosition = VectorF(0.0f, 0.0f);
	camera->setPosition(cameraPosition);

	// TODO: fix these values
	camera->initShakeyCam(100.0f, 80.0f);

	RectF* playerRect = &mGameData->environment->actors()->player()->get()->rectRef();
	camera->follow(playerRect);
}


void GameState::initRendering()
{
	mGameData->renderManager->Set(mGameData->environment);
	mGameData->renderManager->Set(mGameData->uiManager);
}
