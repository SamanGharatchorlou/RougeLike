#pragma once

#include "Screen.h"

class PopupScreen : public Screen
{
public:
	PopupScreen() { }

	void build(const XMLNode& node);

	void init() override;
	void handleInput(const InputManager* input);
	void slowUpdate() override;
	void exit() override { }

	ScreenType type() { return ScreenType::Popup; }


	void setMainText(const BasicString& text);
private:
	void setTitle(const XMLNode& titleNode);
	void setBody(const XMLNode& bodyNode);

private:
	void exitPopup();

};