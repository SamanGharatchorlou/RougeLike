#pragma once

#include "Events/Observer.h"
#include "ScreenController.h"
#include "Objects/Pools/ScreenPool.h"
#if UI_EDITOR
#include "Debug/UIEditor.h"
#endif

class GameController;
class InputManager;
class Cursor;
class Screen;

class UIManager : public Observer
{
	friend class ScreenController;

public:
	UIManager() : mCursor(nullptr) { }
	~UIManager();

	void init(GameController* gameController);
	void preLoad();
	void load();
	void clearScreens();

	void initCursor(Cursor* cursor);

	void handleInput(const InputManager* input);
	void update();
	void render();

	void handleEvent(EventData& data) override;
	
	Screen* screen(ScreenType type);
	Screen* getActiveScreen();

	void setCursorTexture(Texture* texture);

	ScreenController* controller() { return &mController; }

private:
	Cursor* mCursor;

	ScreenPool mScreenPool;

	ScreenController mController;

#if UI_EDITOR
	UIEditor mEditor;
#endif
};