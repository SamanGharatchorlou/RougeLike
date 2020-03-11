#include "pch.h"
#include "PreGameState.h"

#include "GameState.h"

#include "Managers/GameController.h"
#include "UI/UIManager.h"
#include "Graphics/RenderManager.h"
#include "Game/Cursor.h"

#include "Graphics/TextureManager.h"
#include "UI/Screens/CharacterSelectionScreen.h"

#include "Characters/Player/PlayerManager.h"


PreGameState::PreGameState(GameData* gameData, GameController* gameController) :
	mGameData(gameData), mGameController(gameController)
{

}


void PreGameState::init()
{
	mGameData->uiManager->selectScreen(Screen::CharacterSelection);
	selectionScreen = static_cast<CharacterSelectionScreen*>(mGameData->uiManager->getActiveScreen());
	
	mGameData->cursor->setTexture(mGameData->textureManager->getTexture("UICursor"));

	// Player
	mGameData->playerManager->init();
}


void PreGameState::slowUpdate(float dt)
{
	if (selectionScreen->enterGame())
	{
		selectCharacter();
		enterGame();
	}
}


void PreGameState::selectCharacter()
{
	mGameData->playerManager->selectCharacter(selectionScreen->selectCharacter());
}


void PreGameState::enterGame()
{
	mGameController->getStateMachine()->replaceState(new GameState(mGameData, mGameController));
	mGameData->uiManager->selectScreen(Screen::Game);
}



void PreGameState::render()
{
	SDL_Renderer* renderer = Renderer::Get()->sdlRenderer();

	// clear screen
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(renderer);


	mGameData->uiManager->render();

	mGameData->cursor->render();

	// update window surface
	SDL_RenderPresent(renderer);
}