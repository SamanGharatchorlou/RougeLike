#pragma once

#include "Screen.h"

class LoadingScreen : public Screen
{
public:
	LoadingScreen() { }

	void init() override;
	void handleInput(const InputManager* input) { };
	void slowUpdate() override { };
	void exit() override { }

	ScreenType type() override { return ScreenType::LoadingScreen; }
};