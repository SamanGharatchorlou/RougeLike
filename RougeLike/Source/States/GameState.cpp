#include "pch.h"
#include "GameState.h"
#include "PauseState.h"

#include "Managers/GameController.h"
#include "Graphics/TextureManager.h"
#include "Graphics/RenderManager.h"
#include "Input/InputManager.h"
#include "UI/UIManager.h"
#include "Managers/ScoreManager.h"

#include "Game/Cursor.h"
#include "Game/Camera.h"

#include "Characters/Enemies/Enemy.h"

// TESTING
#include "Map/Map.h"
#include "Graphics/Texture.h"
#include "Graphics/Font.h"

GameState::GameState(GameData* gameData, GameController* gameController) : 
	mGameData(gameData)
	, mGameController(gameController)
	, mPlayer(gameData)
	, mEnemies(gameData)
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
	mGameData->uiManager->selectMenu(UIManager::game);

	// Player
	mPlayer.init();
	mGameData->camera->follow(&mPlayer.getMovement());

	// Enemies
	mEnemies.addEnemiesToPool(EnemyType::Imp, 10);
	mEnemies.subscribePlayerWeaponCollisions(&mPlayer.getWeapon());

	// rendering
	mGameData->renderManager->Set(mGameData->map);
	mGameData->renderManager->Set(&mPlayer);
	mGameData->renderManager->Set(&mEnemies);
	mGameData->renderManager->Set(mGameData->uiManager);

	mEnemies.spawn(EnemyType::Imp, 5.0f);
	mEnemies.spawn(EnemyType::Imp, 10.0f);
	mEnemies.spawn(EnemyType::Imp, 15.0f);
	mEnemies.spawn(EnemyType::Imp, 20.0f);
	mEnemies.spawn(EnemyType::Imp, 22.0f);
	mEnemies.spawn(EnemyType::Imp, 25.0f);
	mEnemies.spawn(EnemyType::Imp, 28.0f);
	mEnemies.spawn(EnemyType::Imp, 35.0f);
	mEnemies.spawn(EnemyType::Imp, 40.0f);
	mEnemies.spawn(EnemyType::Imp, 42.0f);
	mEnemies.spawn(EnemyType::Imp, 45.0f);
}


void GameState::preProcess()
{
	mPlayer.processStateChanges();
}


void GameState::handleInput()
{
	if (mGameData->inputManager->isPressed(Button::ESC))
	{
		mGameController->quitGame();
	}
	
	if (mGameData->inputManager->isPressed(Button::PAUSE))
	{
		mGameController->getStateMachine()->addState(new PauseState(mGameData, mGameController));
	}

	mPlayer.handleInput();
	mGameData->uiManager->handleInput();
}

void GameState::slowUpdate(float dt)
{
	mPlayer.slowUpdate(dt);
	mEnemies.slowUpdate(dt);
}

void GameState::fastUpdate(float dt)
{
	// resolve collisions before any movement takes place!
	mPlayer.resolveWallCollisions(dt);

	mPlayer.fastUpdate(dt);
	mEnemies.fastUpdate(dt);

	mGameData->camera->update();
}



void GameState::render()
{
	// clear screen
	SDL_SetRenderDrawColor(mGameData->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(mGameData->renderer);

	mGameData->renderManager->render();

	Font* font = mGameData->textureManager->getFont("LazyFont");

	SDL_Color textColor = { 255, 0, 0 };
	font->setText("Test Test", textColor);
	font->render(RectF(100, 100, 500, 300));

	// update window surface
	SDL_RenderPresent(mGameData->renderer);
}


void GameState::resume()
{
	mGameData->cursor->setTexture(mGameData->textureManager->getTexture("GameCursor"));
}