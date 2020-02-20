#include "pch.h"
#include "PauseState.h"
#include "GameState.h"

#include "Managers/GameController.h"
#include "Graphics/TextureManager.h"
#include "Input/InputManager.h"
#include "Graphics/RenderManager.h"

#include "Game/Cursor.h"

#include "UI/UIManager.h"
#include "UI/Screens/PauseScreen.h"


PauseState::PauseState(GameData* gameData, GameController* gameController) : 
	mGameData(gameData), mGameController(gameController) { }


void PauseState::init() 
{
	mGameData->uiManager->selectScreen(Screen::Pause);
	pauseScreen = static_cast<PauseScreen*>(mGameData->uiManager->getActiveScreen());

	mGameData->cursor->setTexture(mGameData->textureManager->getTexture("UICursor"));
}


void PauseState::slowUpdate(float dt)
{
	if (pauseScreen->resumeGame())
	{
		resumeGame();
	}
	else if (pauseScreen->quitGame())
	{
		quitGame();
	}
	else if (pauseScreen->restartGame())
	{
		restartGame();
	}
}


void PauseState::handleInput()
{
	if (mGameData->inputManager->isPressed(Button::ESC) ||
		mGameData->inputManager->isPressed(Button::QUIT))
	{
		quitGame();
	}

	if (mGameData->inputManager->isPressed(Button::PAUSE))
	{
		resumeGame();
	}
}


void PauseState::render() 
{
	// clear screen
	SDL_SetRenderDrawColor(mGameData->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(mGameData->renderer);

	mGameData->renderManager->render();

	// update window surface
	SDL_RenderPresent(mGameData->renderer);
}


void PauseState::quitGame()
{
	mGameController->quitGame();
}


void PauseState::resumeGame()
{
	mGameController->getStateMachine()->popState();
	mGameData->uiManager->selectScreen(Screen::Game);
}


void PauseState::restartGame()
{
	mGameController->restartGame();
}