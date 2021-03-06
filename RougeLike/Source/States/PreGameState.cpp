 #include "pch.h"
#include "PreGameState.h"

#include "Graphics/Renderer.h"
#include "Game/Data/GameData.h"

#include "Audio/AudioManager.h"
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

	AudioManager* audio = AudioManager::Get();
	if(!audio->isPlaying("Menu", nullptr))
		audio->pushEvent(AudioEvent(AudioEvent::FadeIn, "Menu", nullptr, 1000));
}


void PreGameState::slowUpdate(float dt)
{
	Screen* screen = mGameData->uiManager->getActiveScreen();

	if (screen->type() == ScreenType::CharacterSelection)
	{
		CharacterSelectionScreen* selectionScreen = static_cast<CharacterSelectionScreen*>(screen);
		if(selectionScreen->characterSelected())
		{
			mSelectedCharacter = selectionScreen->selectedCharacter();
			mSelectedWeapon = selectionScreen->selectedWeapon();
		}
	}
}


void PreGameState::exit()
{
	PlayerManager* player = mGameData->environment->actors()->player();

	if(!mSelectedCharacter.empty())
		player->selectCharacter(mSelectedCharacter);

	if(!mSelectedWeapon.empty())
		player->selectWeapon(mSelectedWeapon);
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
