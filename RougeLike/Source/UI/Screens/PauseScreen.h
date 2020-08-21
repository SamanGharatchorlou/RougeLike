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
	~PauseScreen() { mButtons.clear(); }

	void enter() override;
	void update() override { };
	void exit() override { };

	Type type() override { return Type::Pause; }


private:
	std::unordered_map<Option, UIButton*> mButtons;
};