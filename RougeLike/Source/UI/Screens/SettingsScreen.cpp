#include "pch.h"
#include "SettingsScreen.h"

#include "Game/GameData.h"
#include "Input/InputManager.h"
#include "UI/UIManager.h"
#include "UI/Elements/UIButton.h"

#include "Audio/AudioManager.h"


SettingsScreen::SettingsScreen(GameData* gameData) : Screen(gameData) 
{
	mSliders.push_back(UISlider());
	mSliders.push_back(UISlider());
}


void SettingsScreen::enter()
{
	UIButton* musicSlider = mGameData->uiManager->findButton("MusicSlider");
	UIElement* musicBar = mGameData->uiManager->findElement("MusicBarGreen");
	UIElement* musicContainer = mGameData->uiManager->findElement("MusicBarEmpty");
	mSliders[Music].setComponents(musicSlider, musicBar, musicContainer);
	mSliders[Music].setValue(mGameData->audioManager->musicVolume());

	UIButton* soundSlider = mGameData->uiManager->findButton("SoundSlider");
	UIElement* soundBar = mGameData->uiManager->findElement("SoundBarGreen");
	UIElement* soundContainer = mGameData->uiManager->findElement("SoundBarEmpty");
	mSliders[Sound].setComponents(soundSlider, soundBar, soundContainer);
	mSliders[Sound].setValue(mGameData->audioManager->soundVolume());
}


void SettingsScreen::update(float dt)
{
	for(int i = 0; i < Setting::Count; i++)
	{
		// Activate slider
		if (mSliders[i].isPressed())
		{
			mSliders[i].setActive(true);
			mSliders[i].setCursorOffset(mGameData->inputManager->cursorPosition().x);
		}

		// SetSlider position
		if (mSliders[i].isActive())
		{
			mSliders[i].setPosition(mGameData->inputManager->cursorPosition().x);

			if (i == Music)
				mGameData->audioManager->setMusicVolume(mSliders[i].value());
			else if (i == Sound)
				mGameData->audioManager->setSoundVolume(mSliders[i].value());
		}

		// Deactivate slider
		if (mGameData->inputManager->isCursorReleased(Cursor::Left))
		{
			mSliders[i].setActive(false);
		}
	}


	// TODO: create button on/off functionality
	UIButton* muteButton = mGameData->uiManager->findButton("MuteTick");
	if (!muteButton->isActive())
	{
		muteButton->setVisibility(false);
	}
	else
	{
		muteButton->setVisibility(true);

		if (muteButton->isPressed())
		{
			muteButton->setActive(false);
		}
	}

	UIButton* closeButton = mGameData->uiManager->findButton("CloseButton");
	if (closeButton->isReleased())
	{
		mGameData->uiManager->selectScreen(Screen::Pause);
	}
}


void SettingsScreen::setMusicVolume(float volume)
{
	mSliders[Music].setValue(volume);
}

void SettingsScreen::setSoundVolume(float volume)
{
	mSliders[Sound].setValue(volume);
}