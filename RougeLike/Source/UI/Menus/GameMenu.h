#pragma once

#include "Menu.h"

struct GameData;

class GameMenu : public Menu
{
public:
	GameMenu(GameData* gameData) : Menu(gameData) { }

	void enter() override {}
	void update() override {}
	void exit() override { }
};