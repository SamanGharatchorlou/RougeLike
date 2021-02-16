#include "pch.h"
#include "GameState.h"

#include "Graphics/Renderer.h"
#include "Game/Data/GameData.h"
#include "Game/Environment.h"

#include "Input/InputManager.h"
#include "Audio/AudioManager.h"
#include "Graphics/RenderManager.h"
#include "Managers/ScoreManager.h"
#include "Actors/ActorManager.h"
#include "Actors/Player/Player.h"

#include "Map/Map.h"
#include "Game/Camera/Camera.h"


#include "Networking/NetworkManager.h"


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
	audio->pushEvent(AudioEvent(AudioEvent::FadeIn, "Game", nullptr, 1500));
	audio->setSource(mGameData->environment->actors()->player()->get(), Camera::Get()->size().x);

	// open network threads
	if (mGameData->network->mServer)
	{
		mGameData->network->listenForMessages();
	}
	else if (mGameData->network->mClient)
	{
		mGameData->network->sendMessages();
	}
}


void GameState::handleInput()
{
	mGameData->environment->handleInput(mGameData->inputManager);

	int movement = mGameData->network->handleNetworkInput();
	if (movement > -1)
	{
		PlayerManager* player = mGameData->environment->actors()->player();

		VectorF position = player->position();

		float x = 0.0f;
		float y = 0.0f;
		float speed = 1.0f;

		if (movement == 1)
		{
			x = -speed;
		}

		if (movement == 2)
		{
			x = speed;
		}

		if (movement == 3)
		{
			y = -speed;
		}

		if (movement == 4)
		{
			y = speed;
		}

		position += VectorF(x, y);

		player->setPosition(position);
	}



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

	Cursor* cursor = mGameData->inputManager->getCursor();
	cursor->mode();
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
	AudioManager::Get()->pushEvent(AudioEvent(AudioEvent::FadeIn, "Game", nullptr, 750));
}

void GameState::pause()
{
	mGameData->environment->pause();
	AudioManager::Get()->pushEvent(AudioEvent(AudioEvent::FadeOut, "Game", nullptr, 150));
}


void GameState::exit()
{
	mGameData->environment->clear();
	mGameData->scoreManager->reset();
	AudioManager::Get()->pushEvent(AudioEvent(AudioEvent::FadeOut, "Game", nullptr, 150));
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
