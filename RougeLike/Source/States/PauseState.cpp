#include "pch.h"
#include "PauseState.h"
#include "GameState.h"

#include "Game/GameController.h"
#include "Graphics/TextureManager.h"
#include "Input/InputManager.h"
#include "Graphics/RenderManager.h"

#include "UI/UIManager.h"
#include "UI/Screens/GameScreen.h"
#include "UI/Screens/PauseScreen.h"
#include "UI/Screens/SettingsScreen.h"


PauseState::PauseState(GameData* gameData, GameController* gameController) : 
	mGameData(gameData), mGameController(gameController) { }


void PauseState::init() 
{
	mGameData->uiManager->pushScreen(ScreenType::Pause);
	mGameData->uiManager->setCursorTexture(TextureManager::Get()->getTexture("UICursor", FileManager::Image_UI));
}


void PauseState::handleInput()
{
	if (mGameData->inputManager->isPressed(Button::Esc) ||
		mGameData->inputManager->isPressed(Button::Quit))
	{
		quitGame();
	}
}


void PauseState::slowUpdate(float dt)
{
	Screen* screen = mGameData->uiManager->getActiveScreen();
	if (screen->type() == ScreenType::Pause)
	{
		if (screen->selected(ScreenItem::Quit))
		{
			quitGame();
		}
		else if (screen->selected(ScreenItem::Restart))
		{
			restartGame();
		}
	}
	else if (screen->type() == ScreenType::Game)
	{
		resumeGame();
	}
}


void PauseState::render() 
{
	SDL_Renderer* renderer = Renderer::Get()->sdlRenderer();

	// clear screen
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(renderer);

	mGameData->renderManager->render();

	// update window surface
	SDL_RenderPresent(renderer);
}


void PauseState::quitGame()
{
	mGameController->quitGame();
}


void PauseState::resumeGame()
{
	mGameController->getStateMachine()->popState();
}


void PauseState::restartGame()
{
	mGameController->restartGame();
}