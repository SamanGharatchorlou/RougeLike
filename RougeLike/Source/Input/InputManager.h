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
	void processInputEvent(SDL_Event& event);
	void update();

	// Buttons
	const Button& getButton(Button::Key key) const;

	bool isHeld(Button::Key key) const { return getButton(key).isHeld(); }
	bool isPressed(Button::Key key) const { return getButton(key).isPressed(); }
	bool isReleased(Button::Key key) const { return getButton(key).isReleased(); }

	int getHeldFrames(Button::Key key) const { return getButton(key).getHeldFrames(); }

	// Cursor
	bool isCursorPressed() const;
	bool isCursorReleased() const;
	bool isCursorHeld() const;


private:
	void bindDefaultButtons();


private:
	Button mButtons[maxButtons];
	Cursor* mCursor;
};