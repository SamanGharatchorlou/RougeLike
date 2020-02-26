#include "pch.h"
#include "InputManager.h"

#include "Game/Cursor.h"

void InputManager::init()
{
	bindDefaultButtons();
}


// Must run before processInputEvent
void InputManager::update()
{
	// Reset cursor states
#if _DEBUG
	if ((mCursor->isHeld() || mCursor->isPressed()) && mCursor->isReleased())
	{
		DebugPrint(Warning, "Cursor is being pressed and released at the same time\n");
	}
#endif

	mCursor->setPressed(false);
	mCursor->setReleased(false);

	// Reset button states
	for (Button& button : mButtons)
	{
#if _DEBUG
		if ( (button.isHeld() || button.isPressed()) && button.isReleased())
		{
			DebugPrint(Warning, "Button key %d is being pressed and released at the same time\n", button.getKey());
		}
#endif
		button.setPressed(false);
		button.setReleased(false);

		if (button.isHeld())
			button.incrementHeldFrame();
	}
}


void InputManager::processInputEvent(SDL_Event& event)
{
	// Mouse events
	if (event.type == SDL_MOUSEMOTION)
	{
		int x, y;
		SDL_GetMouseState(&x, &y);

		mCursor->setPosition((float)x, (float)y);
		mCursor->setMotion(true);
	}
	else if (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP)
	{
		if (!mCursor->isHeld())
			mCursor->setPressed(event.type == SDL_MOUSEBUTTONDOWN);

		mCursor->setHeld(event.type == SDL_MOUSEBUTTONDOWN);
		mCursor->setReleased(event.type == SDL_MOUSEBUTTONUP);
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


void InputManager::bindDefaultButtons()
{
	int index = 0;
	mButtons[index++].bindToKey(Button::None);

	// Movement
	mButtons[index++].bindToKey(Button::Up);
	mButtons[index++].bindToKey(Button::Down);
	mButtons[index++].bindToKey(Button::Left);
	mButtons[index++].bindToKey(Button::Right);


	// Game state
	mButtons[index++].bindToKey(Button::Pause);
	mButtons[index++].bindToKey(Button::Esc);
	mButtons[index++].bindToKey(Button::Quit);

	ASSERT(Warning, index < maxButtons, "Attempting to bind too many buttons, you need to increase the size of 'maxButtons'\n");
}

// Cursor
bool InputManager::isCursorPressed() const { return mCursor->isPressed(); }
bool InputManager::isCursorReleased() const { return mCursor->isReleased(); }
bool InputManager::isCursorHeld() const { return mCursor->isHeld(); }