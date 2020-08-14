#pragma once

#include "UI/Screens/Screen.h"
#include "Events/Observer.h"
#include "Events/Dispatcher.h"

#if UI_EDITOR
#include "Debug/UIEditor.h"
#endif

class InputManager;
class Cursor;

class UIManager : public Observer
{
public:
	UIManager();
	~UIManager();

	void setupScreens();
	void initCursor(Cursor* cursor);

	void selectScreen(Screen::Type screenType);

	void handleInput(const InputManager* input);
	void update(float dt);
	void render();

	void handleEvent(EventData& data) override;
	
	Screen* screen(Screen::Type type);
	Screen* getActiveScreen() { return mActiveScreen; }

	//bool isUsingUI() const;

	void setCursorTexture(Texture* texture);


private:
	Cursor* mCursor;
	Screen* mActiveScreen;
	std::vector<Screen*> mScreens;

#if UI_EDITOR
	UIEditor mEditor;
#endif
};