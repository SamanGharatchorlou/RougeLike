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
	SettingsScreen(GameData* gameData);

	void update(float dt) override;

	void enter() override;
	void exit() override { };

	Type type() override { return Type::Settings; }

	void setMusicVolume(float volume);
	void setSoundVolume(float volume);

private:
	std::vector<UISlider> mSliders;
};