#pragma once

#include "UI/Screens/Screen.h"
#include "Events/Observer.h"
#include "Events/Dispatcher.h"

#if _DEBUG
#include "Debug/DebugDraw.h"
#endif

struct GameData;
class Texture;
class Cursor;
class UIElement;
class UIButton;

class UIManager : public Observer, public Dispatcher
{
public:
	UIManager(GameData* gameData);
	~UIManager();

	void clearScreens();
	void refresh(Screen::Type screenType);
	void selectScreen(Screen::Type screenType);

	void init();
	void handleInput();
	void update(float dt);
	void render();

	void handleEvent(EventData& data) override;
	
	UIElement* findElement(const std::string& id);
	UIButton* findButton(const std::string& id);

	Screen* getActiveScreen() { return activeScreen; }
	std::string typeToString(Screen::Type screenType);

	bool isUsingUI() const;

	void setCursorTexture(Texture* texture);


private:
	// Event handling functions
	void updateTextBox(UpdateTextBoxEvent& eventData);
	void setUIbar(SetUIBarEvent& eventData);
	void setHealth(SetHealthBarEvent& eventData);	
	void setArmor(SetArmorBarEvent& eventData);
	void moveElement(EditUIRectEvent& eventData);
	void setElementSize(EditUIRectEvent& eventData);
	void setRect(SetUIRectEvent& eventData);
	void setTextColour(SetTextColourEvent& eventData);
	//void setMusicVolume(SetVolumeEvent& eventData);
	//void setGameVolume(SetVolumeEvent& eventData);


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