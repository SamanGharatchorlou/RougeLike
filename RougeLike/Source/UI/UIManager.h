#pragma once

#include "UI/Screens/Screen.h"

struct GameData;

class UIManager
{
public:
	UIManager(GameData* gameData);
	~UIManager();

	void selectScreen(Screen::Type screenType);

	void init();
	void handleInput() { activeScreen->handleInput(); }
	void update() { activeScreen->update(); }
	void render() { activeScreen->render(); }

	Screen* getActiveScreen() { return activeScreen; }

	UIElement* find(std::string id);


private:
	GameData* mGameData;

	Screen* activeScreen;

	std::vector<Screen*> screens;
};