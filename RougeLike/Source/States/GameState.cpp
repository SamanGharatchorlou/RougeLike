#include "pch.h"
#include "GameState.h"

// TODO: clean up these includes
#include "Game/Data/GameData.h"

#include "Audio/AudioManager.h"
#include "Graphics/RenderManager.h"
#include "Input/InputManager.h"
#include "Managers/ScoreManager.h"
#include "Collisions/CollisionManager.h"

#include "Game/Environment.h"
#include "Map/Map.h"
#include "Game/Camera/Camera.h"

#include "Actors/ActorManager.h"
#include "Actors/Player/Player.h"
#include "Actors/Enemies/EnemyManager.h"



#include "Utilities/Quad2D.h"
#include "Collisions/Colliders/QuadCollider.h"


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
	audio->setSoundVolume(0.0f);
}


void GameState::handleInput()
{
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

	//mGameData->scoreManager->slowUpdate();

	Camera::Get()->slowUpdate(dt);

	// End of slow frame
#if !TRACK_COLLISIONS // Moved to end of render function
	mGameData->collisionManager->resetColliders();
#endif
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

#if TRACK_COLLISIONS // Need to keep the hit data until after rendering
	mGameData->collisionManager->resetColliders();
#endif
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
	mGameData->collisionManager->clearColliders();
}


// --- Private Functions --- //

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
