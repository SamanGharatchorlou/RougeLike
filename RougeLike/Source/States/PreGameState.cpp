#include "pch.h"
#include "PreGameState.h"

#include "Game/Data/GameData.h"
#include "GameState.h"

#include "Game/GameController.h"

#include "Input/InputManager.h"
#include "UI/UIManager.h"
#include "Graphics/TextureManager.h"
#include "UI/Screens/CharacterselectionScreen.h"

#include "Game/Environment.h"
#include "Actors/ActorManager.h"
#include "Actors/Player/PlayerManager.h"


PreGameState::PreGameState(GameData* gameData, GameController* gameController) :
	mGameData(gameData), mGameController(gameController) { }


void PreGameState::init()
{	
	UIManager* UI = mGameData->uiManager;
	UI->pushScreen(ScreenType::CharacterSelection);
	
	UI->setCursorTexture(TextureManager::Get()->getTexture("UICursor", FileManager::Image_UI));
}


void PreGameState::slowUpdate(float dt)
{
	if (mGameData->inputManager->isPressed(Button::Esc))
	{
		mGameController->quitGame();
	}

	if (mGameData->uiManager->getActiveScreen()->selected(ScreenItem::Play) || mGameData->inputManager->isPressed(Button::Enter))
	{
		CharacterSelectionScreen* selectionScreen = static_cast<CharacterSelectionScreen*>(mGameData->uiManager->getActiveScreen());
		PlayerManager* player = mGameData->environment->actors()->player();

		player->selectCharacter(selectionScreen->selectedCharacter());
		player->selectWeapon(selectionScreen->selectedWeapon());

		mGameController->getStateMachine()->replaceState(new GameState(mGameData, mGameController));
	}
}


void PreGameState::render()
{
	SDL_Renderer* renderer = Renderer::Get()->sdlRenderer();

	// clear screen
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(renderer);

	mGameData->uiManager->render();

	// update window surface
	SDL_RenderPresent(renderer);
}
