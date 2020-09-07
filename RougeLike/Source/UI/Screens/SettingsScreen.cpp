#include "pch.h"
#include "SettingsScreen.h"

#include "UI/ScreenController.h"
#include "Audio/AudioManager.h"



void SettingsScreen::init()
{
	linkSlider(ScreenItem::Music, "MusicSlider");
	linkSlider(ScreenItem::Sound, "SoundSlider");
	linkButton(ScreenItem::Close, "CloseButton");
	linkButton(ScreenItem::Back, "BackButton");

	AudioManager* audio = AudioManager::Get();

	float musicVolume = audio->musicVolume();
	slider(ScreenItem::Music)->setSliderValue(musicVolume);

	float soundVolume = audio->soundVolume();
	slider(ScreenItem::Sound)->setSliderValue(soundVolume);
}


void SettingsScreen::slowUpdate()
{
	AudioManager* audio = AudioManager::Get();

	UISlider* soundSlider = slider(ScreenItem::Sound);
	if (soundSlider->isActive())
	{
		audio->setSoundVolume(slider(ScreenItem::Sound)->getValue());
	}

	UISlider* musicSlider = slider(ScreenItem::Music);
	if (musicSlider->isActive())
	{
		audio->setMusicVolume(slider(ScreenItem::Music)->getValue());
	}

	if (released(ScreenItem::Close) || released(ScreenItem::Back))
	{
		mController->popScreen();
	}
}