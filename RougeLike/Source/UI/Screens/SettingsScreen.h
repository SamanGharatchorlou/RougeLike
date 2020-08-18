#pragma once

#include "Screen.h"
#include "UI/Elements/UISlider.h"

struct GameData;


class SettingsScreen : public Screen
{
public:
	enum Setting
	{
		Music,
		Sound,
		Mute,
		Close,
		Count
	};

public:

	void enter() override;
	void update(float dt) override;
	void exit() override { };

	Type type() override { return Type::Settings; }


private:
	std::unordered_map<Setting, UISlider*> mSliders;
	std::unordered_map<Setting, UIButton*> mButtons;
};