#pragma once

#include "Screen.h"

class PopupScreen : public Screen
{
public:
	PopupScreen() { }

	void init() override;
	void handleInput(const InputManager* input);
	void slowUpdate() override;
	void exit() override { }

	ScreenType type() { return ScreenType::Popup; }

	void setMainText(const BasicString& text);

private:
	void exitPopup();

};