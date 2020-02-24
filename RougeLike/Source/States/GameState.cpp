#include "pch.h"
#include "GameState.h"
#include "PauseState.h"

#include "Managers/GameController.h"
#include "Graphics/TextureManager.h"
#include "Graphics/RenderManager.h"
#include "Input/InputManager.h"
#include "UI/UIManager.h"
#include "Managers/ScoreManager.h"

#include "Map/Map.h"
#include "Game/Cursor.h"
#include "Game/Camera.h"

#include "Characters/Player/PlayerManager.h"
#include "Characters/Enemies/EnemyManager.h"




GameState::GameState(GameData* gameData, GameController* gameController) : 
	mGameData(gameData)
	, mGameController(gameController)
{
	// random seed
	srand((unsigned int)time(NULL));
}


void GameState::init()
{
	mGameData->cursor->setTexture(mGameData->textureManager->getTexture("GameCursor"));

	// Map
	mGameData->map->setScale(3.0f);
	mGameData->map->generateRandomTunnel(20, 100);

	// Camera
	mGameData->camera->setupCamera(mGameData->map);

	// UI
	mGameData->uiManager->selectScreen(Screen::Game);

	// Player
	PlayerManager* player = mGameData->player;
	EnemyManager* enemies = mGameData->enemies;

	player->init(mGameData->enemies);
	mGameData->camera->follow(player->getRectRef());

	// Enemies
	enemies->addEnemiesToPool(EnemyType::Imp, 10);
	enemies->subscribe(mGameData->player->getWeaponColliders());
	enemies->setTarget(mGameData->player->getRectRef());

	// rendering
	mGameData->renderManager->Set(mGameData->map);
	mGameData->renderManager->Set(player->get());
	mGameData->renderManager->Set(enemies);
	mGameData->renderManager->Set(mGameData->uiManager);


	// Test spawning
	enemies->spawn(EnemyType::Imp, 5.0f);
	//enemies->spawn(EnemyType::Imp, 10.0f);
	//enemies->spawn(EnemyType::Imp, 15.0f);
	//enemies->spawn(EnemyType::Imp, 20.0f);
	//enemies->spawn(EnemyType::Imp, 22.0f);
	//enemies->spawn(EnemyType::Imp, 25.0f);
	//enemies->spawn(EnemyType::Imp, 28.0f);
	//enemies->spawn(EnemyType::Imp, 35.0f);
	//enemies->spawn(EnemyType::Imp, 40.0f);
	//enemies->spawn(EnemyType::Imp, 42.0f);
}


void GameState::preProcess()
{
	mGameData->player->preProcess();
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

	mGameData->player->handleInput();
	mGameData->uiManager->handleInput();
}
 

void GameState::slowUpdate(float dt)
{
	mGameData->player->slowUpdate(dt);
	mGameData->enemies->slowUpdate(dt);

	mGameData->scoreManager->update();
}


void GameState::fastUpdate(float dt)
{
	mGameData->player->fastUpdate(dt);
	mGameData->enemies->fastUpdate(dt);

	mGameData->camera->update();
}


void GameState::render()
{
	// clear screen
	SDL_SetRenderDrawColor(mGameData->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(mGameData->renderer);

	mGameData->renderManager->render();

	// update window surface
	SDL_RenderPresent(mGameData->renderer);
}


void GameState::resume()
{
	mGameData->cursor->setTexture(mGameData->textureManager->getTexture("GameCursor"));
}