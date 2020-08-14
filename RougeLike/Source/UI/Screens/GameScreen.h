#pragma once

#include "Screen.h"


class GameScreen : public Screen
{
public:
	GameScreen();


	void enter() override {}
	void handleInput(const InputManager* input) { }; // TODO: link to hot keys?
	void update(float dt) override { }
	void exit() override { }

	Type type() override { return Type::Game; }


private:
	ScreenLayer* mAdditionalLayer;
};