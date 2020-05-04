#pragma once

#include "UI/Screens/Screen.h"
#include "Events/Observer.h"

struct GameData;
class Cursor;


class UIManager : public Observer
{
public:
	UIManager(GameData* gameData, Cursor* cursor);
	~UIManager();

	void refresh(Screen::Type screenType);
	void selectScreen(Screen::Type screenType);

	void init();
	void handleInput();
	void update() { activeScreen->update(); }
	void render();

	void handleEvent(const Event event, EventData& data) override;

	UIElement* find(const std::string& id);
	Screen* getActiveScreen() { return activeScreen; }
	std::string typeToString(Screen::Type screenType);

	void setCursorTexture(Texture* texture);

private:
	GameData* mGameData;
	Cursor* mCursor;

	Screen* activeScreen;
	std::vector<Screen*> screens;

#if UI_EDITOR
	std::string elementId;
#endif
};