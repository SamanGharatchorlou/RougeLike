#include "pch.h"
#include "SettingsScreen.h"

#include "Input/InputManager.h"
#include "UI/UIManager.h"
#include "UI/Elements/UIButton.h"

#include "Audio/AudioManager.h"



SettingsScreen::SettingsScreen(const TextureManager* textures) : Screen(textures)
{
	mSliders.push_back(UISlider());
	mSliders.push_back(UISlider());
}


void SettingsScreen::enter()
{
	UIButton* musicSlider = findButton("MusicSlider");
	UIElement* musicBar = find("MusicBarGreen");
	UIElement* musicContainer = find("MusicBarEmpty");

	mSliders[Music].setComponents(musicSlider, musicBar, musicContainer);
	//mSliders[Music].setValue(mGameData->audioManager->musicVolume());

	UIButton* soundSlider = findButton("SoundSlider");
	UIElement* soundBar = find("SoundBarGreen");
	UIElement* soundContainer = find("SoundBarEmpty");

	mSliders[Sound].setComponents(soundSlider, soundBar, soundContainer);
	//mSliders[Sound].setValue(mGameData->audioManager->soundVolume());
}

void SettingsScreen::handleInput(const InputManager* input)
{
	for (int i = 0; i < Setting::Count; i++)
	{
		// Activate slider
		if (mSliders[i].isPressed())
		{
			mSliders[i].setActive(true);
			mSliders[i].setCursorOffset(input->cursorPosition().x);
		}

		// SetSlider position
		if (mSliders[i].isActive())
		{
			mSliders[i].setPosition(input->cursorPosition().x);

			//if (i == Music)
			//	mGameData->audioManager->setMusicVolume(mSliders[i].value());
			//else if (i == Sound)
			//	mGameData->audioManager->setSoundVolume(mSliders[i].value());
		}

		// Deactivate slider
		if (input->isCursorReleased(Cursor::Left))
		{
			mSliders[i].setActive(false);
		}
	}
}


void SettingsScreen::update(float dt)
{
	// TODO: create button on/off functionality
	UIButton* muteButton = findButton("MuteTick");
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

	UIButton* closeButton = findButton("CloseButton");
	if (closeButton->isReleased())
	{
		DebugPrint(Log, "Unimplemented\n");
		//mGameData->uiManager->selectScreen(Screen::Pause);
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