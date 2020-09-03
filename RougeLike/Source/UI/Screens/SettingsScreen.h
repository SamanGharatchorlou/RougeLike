#pragma once

#include "Screen.h"
#include "UI/Elements/UISlider.h"


class SettingsScreen : public Screen
{
public:
	SettingsScreen() { }

	void init() override;
	void slowUpdate() override;
	void exit() override { };

	ScreenType type() override { return ScreenType::Settings; }
};