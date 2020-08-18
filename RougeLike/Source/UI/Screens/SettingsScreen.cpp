#include "pch.h"
#include "SettingsScreen.h"

#include "Input/InputManager.h"
#include "UI/UIManager.h"
#include "UI/Elements/UIButton.h"
#include "UI/Elements/UISlider.h"

#include "Audio/AudioManager.h"





void SettingsScreen::enter()
{
	linkSlider(ScreenItem::Music, "MusicSlider");
	linkSlider(ScreenItem::Sound, "SoundSlider");

	linkButton(ScreenItem::Close, "CloseButton");


	AudioManager* audio = AudioManager::Get();
	float musicVolume = audio->musicVolume();
	mSliders[Setting::Music]->setSliderValue(musicVolume);

	float soundVolume = audio->soundVolume();
	mSliders[Setting::Sound]->setSliderValue(soundVolume);
}


void SettingsScreen::update(float dt)
{
	AudioManager* audio = AudioManager::Get();
	audio->setMusicVolume(mSliders[Setting::Music]->getValue());
	audio->setSoundVolume(mSliders[Setting::Sound]->getValue());
}