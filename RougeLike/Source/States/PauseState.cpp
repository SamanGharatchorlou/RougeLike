#include "pch.h"
#include "PauseState.h"

#include "Audio/AudioManager.h"
#include "Graphics/Renderer.h"
#include "Game/Data/GameData.h"

#include "Graphics/TextureManager.h"
#include "Graphics/RenderManager.h"

#include "UI/UIManager.h"


PauseState::PauseState(GameData* gameData) : mGameData(gameData) { }


void PauseState::init() 
{
	mGameData->uiManager->setCursorTexture(TextureManager::Get()->getTexture("UICursor", FileManager::Image_UI));

	AudioManager* audio = AudioManager::Get();
	AudioManager::Get()->pushEvent(AudioEvent(AudioEvent::FadeIn, "Menu", nullptr, 750));
}


void PauseState::handleInput()
{

}


void PauseState::slowUpdate(float dt)
{

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


void PauseState::exit()
{
	AudioManager::Get()->pushEvent(AudioEvent(AudioEvent::FadeOut, "Menu", nullptr, 150));
}