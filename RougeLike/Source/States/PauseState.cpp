#include "pch.h"
#include "PauseState.h"
#include "GameState.h"

#include "Game/GameController.h"
#include "Graphics/TextureManager.h"
#include "Input/InputManager.h"
#include "Graphics/RenderManager.h"

#include "UI/UIManager.h"
#include "UI/Screens/PauseScreen.h"
#include "UI/Screens/SettingsScreen.h"


PauseState::PauseState(GameData* gameData, GameController* gameController) : 
	mGameData(gameData), mGameController(gameController) { }


void PauseState::init() 
{
	selectScreen(Screen::Pause);
	mGameData->uiManager->setCursorTexture(TextureManager::Get()->getTexture("UICursor", FileManager::Image_UI));

}


void PauseState::handleInput()
{
	if (mGameData->inputManager->isPressed(Button::Esc) ||
		mGameData->inputManager->isPressed(Button::Quit))
	{
		quitGame();
	}

	if (mGameData->inputManager->isPressed(Button::Pause))
	{
		resumeGame();
	}
}


void PauseState::slowUpdate(float dt)
{
	if (mPauseScreen)
	{
		pauseScreenUpdate();
	}
	else if (mSettingsScreen)
	{
		settingsScreenUpdate();
	}
}

void PauseState::pauseScreenUpdate()
{
	if (mPauseScreen->selected(ScreenItem::Resume))
	{
		resumeGame();
	}
	else if (mPauseScreen->selected(ScreenItem::Quit))
	{
		quitGame();
	}
	else if (mPauseScreen->selected(ScreenItem::Restart))
	{
		restartGame();
	}
	else if (mPauseScreen->selected(ScreenItem::Settings))
	{
		selectScreen(Screen::Settings);
	}
}

void PauseState::settingsScreenUpdate()
{
	if (mSettingsScreen->selected(ScreenItem::Close))
	{
		selectScreen(Screen::Pause);
	}
}


void PauseState::selectScreen(Screen::Type screen)
{
	mGameData->uiManager->selectScreen(screen);

	if (screen == Screen::Settings)
	{
		mSettingsScreen = static_cast<SettingsScreen*>(mGameData->uiManager->getActiveScreen());
		mPauseScreen = nullptr;
	}
	else if (screen == Screen::Pause)
	{
		mPauseScreen = static_cast<PauseScreen*>(mGameData->uiManager->getActiveScreen());
		mSettingsScreen = nullptr;
	}
	else
	{
		DebugPrint(Warning, "Screen type %d not recognised by the pause state\n", (int)screen);
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
	mGameData->uiManager->selectScreen(Screen::Game);
}


void PauseState::restartGame()
{
	mGameController->restartGame();
}