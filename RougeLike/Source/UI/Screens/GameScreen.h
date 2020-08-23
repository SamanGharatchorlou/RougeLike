#pragma once

#include "Screen.h"

#include "Objects/Abilities/AbilityClasses/AbilityStates.h"
#include "Input/Button.h"


class GameScreen : public Screen
{
public:

	GameScreen(ScreenController* controller);

	void enter() override;
	void handleInput(const InputManager* input);
	void update() override;
	void exit() override { }

	ScreenType type() override { return ScreenType::Game; }
};