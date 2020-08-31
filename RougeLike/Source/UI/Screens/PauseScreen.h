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
	PauseScreen() { }

	void init() override;
	void handleInput(const InputManager* input);
	void slowUpdate() override;
	void exit() override { };

	ScreenType type() override { return ScreenType::Pause; }


private:
	void popScreenState();


private:
	std::unordered_map<Option, UIButton*> mButtons;
};