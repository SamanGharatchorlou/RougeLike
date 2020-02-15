#include "pch.h"
#include "PauseState.h"
#include "GameState.h"

#include "Managers/GameController.h"
#include "Graphics/TextureManager.h"
#include "Input/InputManager.h"
#include "Graphics/RenderManager.h"

#include "Game/Cursor.h"

#include "UI/UIManager.h"
#include "UI/Menus/PauseMenu.h"


PauseState::PauseState(GameData* gameData, GameController* gameController) : 
	mGameData(gameData), mGameController(gameController) { }


void PauseState::init() 
{
	mGameData->uiManager->selectMenu(UIManager::pause);
	pauseMenu = static_cast<PauseMenu*>(mGameData->uiManager->getActiveMenu());

	mGameData->cursor->setTexture(mGameData->textureManager->getTexture("UICursor"));
}

void PauseState::slowUpdate(float dt)
{
	if (pauseMenu->shouldResumeGame())
	{
		mGameController->getStateMachine()->popState();
	}
	else if (pauseMenu->shouldQuitGame())
	{
		mGameController->quitGame();
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



void PauseState::exit() 
{
}


void PauseState::quitGame()
{
	mGameController->quitGame();
}

void PauseState::resumeGame()
{
	mGameController->getStateMachine()->popState();
	mGameData->uiManager->selectMenu(UIManager::game);
}