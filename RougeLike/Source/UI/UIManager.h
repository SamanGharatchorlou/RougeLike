#pragma once

#include "UI/Screens/Screen.h"
#include "Events/Observer.h"

struct GameData;

class UIManager : public Observer
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


	void handleEvent(Event event, EventData& data) override;

	UIElement* find(std::string id);


private:
	GameData* mGameData;

	Screen* activeScreen;

	std::vector<Screen*> screens;
};