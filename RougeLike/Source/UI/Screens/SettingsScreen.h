#pragma once

#include "Screen.h"
#include "UI/Elements/UISlider.h"

struct GameData;


class SettingsScreen : public Screen
{
public:

	SettingsScreen(ScreenController* controller) : Screen(controller) { }
	~SettingsScreen() { mSliders.clear(); mButtons.clear(); }
	void enter() override;
	void update() override;
	void exit() override { };

	ScreenType type() override { return ScreenType::Settings; }
};