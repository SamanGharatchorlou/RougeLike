#include "pch.h"
#include "SettingsScreen.h"

#include "UI/ScreenController.h"
#include "Audio/AudioManager.h"



void SettingsScreen::init()
{
	linkSlider(ScreenItem::Music, "MusicSlider");
	linkSlider(ScreenItem::Sound, "SoundSlider");
	linkButton(ScreenItem::Close, "CloseButton");

	AudioManager* audio = AudioManager::Get();

	float musicVolume = audio->musicVolume();
	slider(ScreenItem::Music)->setSliderValue(musicVolume);

	float soundVolume = audio->soundVolume();
	slider(ScreenItem::Sound)->setSliderValue(soundVolume);
}


void SettingsScreen::slowUpdate()
{
	AudioManager* audio = AudioManager::Get();
	audio->setMusicVolume(slider(ScreenItem::Music)->getValue());
	audio->setSoundVolume(slider(ScreenItem::Sound)->getValue());

	if (selected(ScreenItem::Close))
	{
		mController->popScreen();
	}
}