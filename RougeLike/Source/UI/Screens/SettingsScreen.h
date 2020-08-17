#pragma once

#include "Screen.h"
#include "UI/Elements/UISlider.h"

struct GameData;


class SettingsScreen : public Screen
{
	enum Setting
	{
		Music,
		Sound,
		Count
	};

public:
	SettingsScreen();

	void enter() override;
	void handleInput(const InputManager* input) override { }
	void update(float dt) override;
	void exit() override { };

	Type type() override { return Type::Settings; }

	void setMusicVolume(float volume);
	void setSoundVolume(float volume);

private:
	// TODO change to map
	std::vector<UISlider> mSliders;
};