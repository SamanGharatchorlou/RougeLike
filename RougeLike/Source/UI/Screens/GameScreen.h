#pragma once

#include "Screen.h"

#include "Objects/Abilities/AbilityClasses/AbilityStates.h"
#include "Input/Button.h"


class GameScreen : public Screen
{
public:
	enum Components
	{
		None,
		Health,
		Armor
	};

public:
	GameScreen();


	void enter() override;
	void handleInput(const InputManager* input);
	void update() override;
	void exit() override { }

	Type type() override { return Type::Game; }


private:
	ScreenLayer* mAdditionalLayer;

};