#pragma once

#include "Screen.h"

struct GameData;

class PauseScreen : public Screen
{
public:
	enum Option
	{
		None,
		Resume,
		Settings,
		Restart,
		Quit
	};

public:

	PauseScreen(ScreenController* controller) : Screen(controller) { }
	~PauseScreen() { mButtons.clear(); }

	void enter() override;
	void handleInput(const InputManager* input);
	void update() override;
	void exit() override { };

	ScreenType type() override { return ScreenType::Pause; }


private:
	std::unordered_map<Option, UIButton*> mButtons;
};