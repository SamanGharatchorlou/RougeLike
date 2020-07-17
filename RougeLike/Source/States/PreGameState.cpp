#include "pch.h"
#include "PreGameState.h"

#include "GameState.h"

#include "Managers/GameController.h"

#include "Input/InputManager.h"
#include "Game/Cursor.h"
#include "UI/UIManager.h"
#include "Collisions/CollisionManager.h"
#include "Graphics/RenderManager.h"

#include "Graphics/TextureManager.h"
#include "UI/Screens/CharacterselectionScreen.h"

#include "Actors/ActorManager.h"
#include "Actors/Player/Player.h"

// TEMP
#include "Map/Environment.h"
#include "Animations/AnimationReader.h"

PreGameState::PreGameState(GameData* gameData, GameController* gameController) :
	mGameData(gameData), mGameController(gameController)
{

}


void PreGameState::init()
{	
	// Setup collision tracking (before setting up any Objects)
	initCollisionTrackers();

	mGameData->uiManager->selectScreen(Screen::CharacterSelection);
	mSelectionScreen = static_cast<CharacterSelectionScreen*>(mGameData->uiManager->getActiveScreen());
	
	mGameData->uiManager->setCursorTexture(mGameData->textureManager->getTexture("UICursor", FileManager::Image_UI));

	//mGameData->actors->player()->loadWeaponStash();
}


void PreGameState::slowUpdate(float dt)
{
	if (mGameData->inputManager->isPressed(Button::Esc))
	{
		mGameController->quitGame();
	}

	if (mSelectionScreen->enterGame())
	{
		mGameData->environment->actors()->player()->selectCharacter(mSelectionScreen->selectedCharacter());
		mGameData->environment->actors()->player()->selectWeapon(mSelectionScreen->selectedWeapon());

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


// --- Private Functions --- //

void PreGameState::initCollisionTrackers()
{

}