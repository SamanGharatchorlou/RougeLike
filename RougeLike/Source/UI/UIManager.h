#pragma once

#include "UI/Screens/Screen.h"
#include "Screens/ScreenController.h"
#include "Events/Observer.h"

#if UI_EDITOR
#include "Debug/UIEditor.h"
#endif

class InputManager;
class Cursor;

class UIManager : public Observer
{
public:
	UIManager() : mCursor(nullptr), mActiveScreen(nullptr) { }
	~UIManager();

	void init();
	void setupScreens();
	void clearScreens();

	void initCursor(Cursor* cursor);

	void clearScreenStack() { mController.clearStack(); }
	void pushScreen(ScreenType screenType);
	void popScreen() { mController.popScreen(); }

	void handleInput(const InputManager* input);
	void update(float dt);
	void render();

	void handleEvent(EventData& data) override;
	
	Screen* screen(ScreenType type);
	Screen* getActiveScreen() { return mActiveScreen; }

	void setCursorTexture(Texture* texture);


private:

	void selectScreen(ScreenType screenType);


private:
	Cursor* mCursor;
	Screen* mActiveScreen;
	std::vector<Screen*> mScreens;

	ScreenController mController;

#if UI_EDITOR
	UIEditor mEditor;
#endif
};