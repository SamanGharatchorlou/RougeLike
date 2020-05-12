#pragma once

#include "UI/Screens/Screen.h"
#include "Events/Observer.h"

struct GameData;
class Texture;
class Cursor;
class UIElement;
class UIButton;

class UIManager : public Observer
{
public:
	UIManager(GameData* gameData);
	~UIManager();

	void clearScreens();
	void refresh(Screen::Type screenType);
	void selectScreen(Screen::Type screenType);

	void init();
	void handleInput();
	void update();
	void render();

	void handleEvent(const Event event, EventData& data) override;
	
	UIElement* findElement(const std::string& id);
	UIButton* findButton(const std::string& id);

	Screen* getActiveScreen() { return activeScreen; }
	std::string typeToString(Screen::Type screenType);

	void setCursorTexture(Texture* texture);

private:
	GameData* mGameData;
	Cursor* mCursor;

	Screen* activeScreen;
	std::vector<Screen*> screens;


#if UI_EDITOR
private:
	void debugEditUI();
	std::string elementId;
#endif
};