#pragma once

#include "Screen.h"

class PopupScreen : public Screen
{
public:
	PopupScreen(ScreenController* controller) : Screen(controller) { }
	void enter() override;
	void update() override { }
	void exit() override { }

	ScreenType type() { return ScreenType::Popup; }
};