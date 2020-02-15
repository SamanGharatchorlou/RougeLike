#pragma once

#include "Menus/PauseMenu.h"
#include "Menus/GameMenu.h"

struct GameData;

class UIManager
{
public:
	enum Screen
	{
		start,
		pause,
		game
	};

public:
	UIManager(GameData* gameData);

	void selectMenu(Screen menu);

	void init();
	void handleInput() { activeMenu->handleInput(); }
	void update() { activeMenu->update(); }
	void render() { activeMenu->render(); }

	Menu* getActiveMenu() { return activeMenu; }


private:
	Menu* activeMenu;

	PauseMenu pauseMenu;
	GameMenu gameMenu;
};