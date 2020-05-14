#include "pch.h"
#include "InputManager.h"

void InputManager::init()
{
	bindDefaultButtons();
}


void InputManager::processInputEvent(SDL_Event& event)
{
	// Mouse movement
	if (event.type == SDL_MOUSEMOTION)
	{
		int x, y;
		SDL_GetMouseState(&x, &y);

		mCursor.setPosition((float)x, (float)y);
		mCursor.setMotion(true);
	}
	// Buttons
	else if (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP)
	{
		SDL_MouseButtonEvent buttonEvent = event.button;

		Cursor::ButtonType buttonType;
		bool isHeld = false;
		bool isPressed = false;
		bool isReleased = false;

		if (buttonEvent.button == Button::LeftClick)
			buttonType = Cursor::Left;
		else if (buttonEvent.button == Button::rightClick)
			buttonType = Cursor::Right;
		else
			DebugPrint(Log, "Mouse button type %d not left or right\n", buttonEvent.button);

		// Get input data
		Button cursorButton = mCursor.getButton(buttonType);

		if (!cursorButton.isHeld())
			isPressed = (event.type == SDL_MOUSEBUTTONDOWN);

		isHeld = (event.type == SDL_MOUSEBUTTONDOWN);
		isReleased = (event.type == SDL_MOUSEBUTTONUP);

		// Set input data
		cursorButton.setHeld(isHeld);
		cursorButton.setPressed(isPressed);
		cursorButton.setReleased(isReleased);
		mCursor.setButton(buttonType, cursorButton);
	}

	// Button events
	else
	{
		for (Button& button : mButtons)
		{
			if (button.isKey(event.key.keysym.sym))
			{
				if (!button.isHeld())
					button.setPressed(event.type == SDL_KEYDOWN);

				button.setHeld(event.type == SDL_KEYDOWN);
				button.setReleased(event.type == SDL_KEYUP);
				break;
			}
		}
	}
}

// TODO: why doesnt this work, input seems to be lost if the moust is moving
/*
{
	resetInputEvents();

	// Mouse movement
	if (event.type == SDL_MOUSEMOTION)
	{
		processMouseMovementEvent(event);
	}
	// Buttons
	if (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP)
	{
		processMouseButtonEvent(event);
	}
	// Button events
	processButtonEvent(event);

}
*/

const Button& InputManager::getButton(Button::Key key) const
{
	for (unsigned int i = 0; i < maxButtons; i++)
	{
		if (mButtons[i].isKey(key))
			return mButtons[i];
	}

	DebugPrint(Warning, "Button for key %d not found, return button with KEY: NONE\n", key);
	return mButtons[0];
}


// Cursor
void InputManager::setCursorSize(VectorF size) { mCursor.setSize(size); }
bool InputManager::isCursorPressed(Cursor::ButtonType button) const { return mCursor.isPressed(button); }
bool InputManager::isCursorReleased(Cursor::ButtonType button) const { return mCursor.isReleased(button); }
bool InputManager::isCursorHeld(Cursor::ButtonType button) const { return mCursor.isHeld(button); }



// --- Private Functions --- //

void InputManager::processMouseMovementEvent(SDL_Event& event)
{
	int x, y;
	SDL_GetMouseState(&x, &y);

	mCursor.setPosition((float)x, (float)y);
	mCursor.setMotion(true);
}


void InputManager::processMouseButtonEvent(SDL_Event& event)
{
	SDL_MouseButtonEvent buttonEvent = event.button;

	Cursor::ButtonType buttonType;
	bool isHeld = false;
	bool isPressed = false;
	bool isReleased = false;

	if (buttonEvent.button == Button::LeftClick)
		buttonType = Cursor::Left;
	else if (buttonEvent.button == Button::rightClick)
		buttonType = Cursor::Right;
	else
		DebugPrint(Log, "Mouse button type %d not left or right\n", buttonEvent.button);

	// Get input data
	Button cursorButton = mCursor.getButton(buttonType);

	if (!cursorButton.isHeld())
		isPressed = (event.type == SDL_MOUSEBUTTONDOWN);

	isHeld = (event.type == SDL_MOUSEBUTTONDOWN);
	isReleased = (event.type == SDL_MOUSEBUTTONUP);

	// Set input data
	cursorButton.setHeld(isHeld);
	cursorButton.setPressed(isPressed);
	cursorButton.setReleased(isReleased);
	mCursor.setButton(buttonType, cursorButton);
}


void InputManager::processButtonEvent(SDL_Event& event)
{
	for (Button& button : mButtons)
	{
		if (button.isKey(event.key.keysym.sym))
		{
			if (!button.isHeld())
				button.setPressed(event.type == SDL_KEYDOWN);

			button.setHeld(event.type == SDL_KEYDOWN);
			button.setReleased(event.type == SDL_KEYUP);
			break;
		}
	}
}


void InputManager::bindDefaultButtons()
{
	int index = 0;
	mButtons[index++].bindToKey(Button::None);

	// Movement
	mButtons[index++].bindToKey(Button::Up);
	mButtons[index++].bindToKey(Button::Down);
	mButtons[index++].bindToKey(Button::Left);
	mButtons[index++].bindToKey(Button::Right);

#if UI_EDITOR
	mButtons[index++].bindToKey(Button::UILeft);
	mButtons[index++].bindToKey(Button::UIRight);
	mButtons[index++].bindToKey(Button::UIDown);
	mButtons[index++].bindToKey(Button::UIUp);
#endif

	// Game state
	mButtons[index++].bindToKey(Button::Pause);
	mButtons[index++].bindToKey(Button::Esc);
	mButtons[index++].bindToKey(Button::Quit);
	mButtons[index++].bindToKey(Button::Enter);
	mButtons[index++].bindToKey(Button::Ctrl);
	mButtons[index++].bindToKey(Button::Space);

	// Keys
	mButtons[index++].bindToKey(Button::E);

	ASSERT(Warning, index < maxButtons, "Attempting to bind too many buttons, you need to increase the size of 'maxButtons'\n");
}


// Must run before processInputEvent
void InputManager::resetInputEvents()
{
	// Reset cursor states
#if _DEBUG
	if (((mCursor.isHeld(Cursor::Left) || mCursor.isPressed(Cursor::Left)) && mCursor.isReleased(Cursor::Left)) ||
		((mCursor.isHeld(Cursor::Right) || mCursor.isPressed(Cursor::Right)) && mCursor.isReleased(Cursor::Right)))
	{
		DebugPrint(Warning, "Cursor is being pressed and released at the same time\n");
	}
#endif

	mCursor.clearInputs();

	// Reset button states
	for (Button& button : mButtons)
	{
#if _DEBUG
		if ((button.isHeld() || button.isPressed()) && button.isReleased())
		{
			DebugPrint(Warning, "Button key %d is being pressed and released at the same time\n", button.key());
		}
#endif

		button.setPressed(false);
		button.setReleased(false);

		if (button.isHeld())
			button.incrementHeldFrames();
		else
			button.setHeldFrames(0);
	}
}