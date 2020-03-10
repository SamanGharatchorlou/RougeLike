#pragma once

#include "Screen.h"

struct GameData;

class GameScreen : public Screen
{
public:
	GameScreen(GameData* gameData, std::vector<UILayer*> layers);

	void enter() override {}
	void update() override { }
	void exit() override { }

	Type type() override { return Type::Game; }

};