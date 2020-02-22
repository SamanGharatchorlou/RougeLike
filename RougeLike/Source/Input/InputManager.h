#pragma once

#include "Button.h"

class Cursor;

static constexpr int maxButtons = 10;

class InputManager
{
public:
	InputManager(Cursor* cursor) : mCursor(cursor) { }
	~InputManager() { }

	void init();
	void processInputEvent(SDL_Event event);
	void update();

	// Buttons
	const Button& getButton(Button::Key key);

	bool isHeld(Button::Key key) { return getButton(key).isHeld(); }
	bool isPressed(Button::Key key) { return getButton(key).isPressed(); }
	bool isReleased(Button::Key key) { return getButton(key).isReleased(); }

	int getHeldFrames(Button::Key key) { return getButton(key).getHeldFrames(); }

	// Cursor
	bool isCursorPressed();
	bool isCursorReleased();
	bool isCursorHeld();


private:
	void bindDefaultButtons();

private:
	Button mButtons[maxButtons];
	Cursor* mCursor;
};