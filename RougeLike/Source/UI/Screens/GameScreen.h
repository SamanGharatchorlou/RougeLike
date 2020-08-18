#pragma once

#include "Screen.h"


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
	void update(float dt) override;
	void exit() override { }

	Type type() override { return Type::Game; }


private:
	ScreenLayer* mAdditionalLayer;
};