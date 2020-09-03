#pragma once

#include "Screen.h"


class GameOverScreen : public Screen
{
public:
	GameOverScreen() { }

	void init() override;
	void slowUpdate() override;
	void exit() override { };

	ScreenType type() override { return ScreenType::GameOver; }

	int mMapLevel;
	int mScore;
	int mKills;
	BasicString mHealth;

private:
	void setTextBox(const BasicString& textbox, int value);
};