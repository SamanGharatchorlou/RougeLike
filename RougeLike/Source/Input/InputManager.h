#pragma once

#include "Button.h"
#include "Cursor.h"


class InputManager
{
public:
	InputManager() { }
	~InputManager() { }

	void init();

	void resetInputEvents();
	void processInputEvent(SDL_Event& event);

	// Buttons
	const Button& getButton(Button::Key key) const;

	Button::State state(Button::Key key) const { return getButton(key).state(); }
	bool isHeld(Button::Key key) const { return getButton(key).isHeld(); }
	bool isPressed(Button::Key key) const { return getButton(key).isPressed(); }
	bool isReleased(Button::Key key) const { return getButton(key).isReleased(); }

	int getHeldFrames(Button::Key key) const { return getButton(key).getHeldFrames(); }

	// Cursor
	Cursor* getCursor() { return &mCursor; }
	const Cursor* getCursor() const { return &mCursor; }

	VectorF cursorPosition() const { return mCursor.position(); }

	void setCursorSize(VectorF size);
	bool isCursorPressed(Cursor::ButtonType type) const;
	bool isCursorReleased(Cursor::ButtonType type) const;
	bool isCursorHeld(Cursor::ButtonType type) const;

	//// TESt
	//void setButtonState(Button::Key key, Button::State state)
	//{
	//	for (Button& button : mButtons)
	//	{


	//		if (button.isKey(event.key.keysym.sym))
	//		{
	//			if (!button.isHeld())
	//				button.setPressed(event.type == SDL_KEYDOWN);

	//			button.setHeld(event.type == SDL_KEYDOWN);
	//			button.setReleased(event.type == SDL_KEYUP);
	//			break;
	//		}
	//	}
	//}

private:
	void processMouseMovementEvent();
	void processMouseButtonEvent(SDL_Event& event);
	void processButtonEvent(SDL_Event& event);

	void bindDefaultButtons();


private:
	std::vector<Button> mButtons;
	Cursor mCursor;
};
