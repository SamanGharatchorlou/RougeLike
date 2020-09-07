 #include "pch.h"
#include "PreGameState.h"

#include "Game/Data/GameData.h"

#include "Input/InputManager.h"
#include "UI/UIManager.h"
#include "Graphics/TextureManager.h"
#include "UI/Screens/CharacterselectionScreen.h"

#include "Game/Environment.h"
#include "Actors/ActorManager.h"
#include "Actors/Player/PlayerManager.h"


PreGameState::PreGameState(GameData* gameData) : mGameData(gameData) { }


void PreGameState::init()
{	
	UIManager* UI = mGameData->uiManager;

	UI->controller()->clearScreenStack();
	UI->controller()->addScreen(ScreenType::CharacterSelection);

	UI->setCursorTexture(TextureManager::Get()->getTexture("UICursor", FileManager::Image_UI));
}


void PreGameState::slowUpdate(float dt)
{
	if (mGameData->uiManager->getActiveScreen()->released(ScreenItem::Play) || mGameData->inputManager->isReleased(Button::Enter))
	{
		CharacterSelectionScreen* selectionScreen = static_cast<CharacterSelectionScreen*>(mGameData->uiManager->getActiveScreen());

		PlayerManager* player = mGameData->environment->actors()->player();

#if DEBUG_CHECK
		if (selectionScreen->selectedCharacter().empty())
			DebugPrint(Warning, "No character has been selected\n");

		if (selectionScreen->selectedWeapon().empty())
			DebugPrint(Warning, "No weapon has been selected\n");
#endif

		player->selectCharacter(selectionScreen->selectedCharacter());
		player->selectWeapon(selectionScreen->selectedWeapon());
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


void PreGameState::exit()
{
	CharacterSelectionScreen* selectionScreen = static_cast<CharacterSelectionScreen*>(mGameData->uiManager->screen(ScreenType::CharacterSelection));
	
	if (selectionScreen)
	{
		PlayerManager* player = mGameData->environment->actors()->player();

	#if DEBUG_CHECK
		if (selectionScreen->selectedCharacter().empty())
			DebugPrint(Warning, "No character has been selected\n");

		if (selectionScreen->selectedWeapon().empty())
			DebugPrint(Warning, "No weapon has been selected\n");
	#endif

		player->selectCharacter(selectionScreen->selectedCharacter());
		player->selectWeapon(selectionScreen->selectedWeapon());

	}
}