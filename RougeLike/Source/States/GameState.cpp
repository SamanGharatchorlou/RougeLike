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

#include "Characters/Player/PlayerManager.h"
#include "Characters/Enemies/Enemy.h"


// TESTING
#include "Map/Map.h"
#include "Graphics/Texture.h"
#include "Graphics/Font.h"
#include "UI/Elements/UIElement.h"
#include "UI/Elements/UITextBox.h"



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
	mGameData->uiManager->selectScreen(Screen::Game);

	// Player
	mPlayer.init(&mEnemies);
	mGameData->camera->follow(mPlayer.getRectRef());

	// Enemies
	mEnemies.addEnemiesToPool(EnemyType::Imp, 10);
	mEnemies.subscribe(mPlayer.getWeaponColliders());
	mEnemies.setTarget(mPlayer.getRectRef());

	// rendering
	mGameData->renderManager->Set(mGameData->map);
	mGameData->renderManager->Set(mPlayer.get());
	mGameData->renderManager->Set(&mEnemies);
	mGameData->renderManager->Set(mGameData->uiManager);


	// Test spawning
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
}


void GameState::preProcess()
{
	mPlayer.preProcess();
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


	//// TESTING -- IT WORKS!!!!
	////update the ui screen with the score
	//int score = mGameData->scoreManager->score();

	//std::string scoreString = "Score: " + std::to_string(score);

	//UIElement* element = mGameData->uiManager->find("Score");

	//if (element != nullptr && element->type() == UIElement::BasicText)
	//{
	//	UIBasicText* text = static_cast<UIBasicText*>(element);

	//	text->setText(scoreString);
	//}
}

void GameState::fastUpdate(float dt)
{
	// resolve collisions before any movement takes place!
	//mPlayer.resolveCollisions(dt);

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

	// update window surface
	SDL_RenderPresent(mGameData->renderer);
}


void GameState::resume()
{
	mGameData->cursor->setTexture(mGameData->textureManager->getTexture("GameCursor"));
}