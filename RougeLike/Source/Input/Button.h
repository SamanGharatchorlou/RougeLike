#pragma once

class Button
{
public:

	enum Key
	{
		None = 0,

		// Mouse
		LeftClick = SDL_BUTTON_LEFT,
		rightClick = SDL_BUTTON_RIGHT,

		// Movement
		Up = SDLK_w,
		Down = SDLK_s,
		Left = SDLK_a,
		Right = SDLK_d,

#if UI_EDITOR
		UILeft = SDLK_LEFT,
		UIRight = SDLK_RIGHT,
		UIUp = SDLK_UP,
		UIDown = SDLK_DOWN,
#endif

		// General Keys
		Pause = SDLK_p,
		Esc = SDLK_ESCAPE,
		Quit = SDL_QUIT,
		Enter = SDLK_RETURN,
		Ctrl = SDLK_LCTRL,

		// Keys
		E = SDLK_e,

		KeyCount
	};


public:
	Button() { mHeldFrames = 0; mHeld = mPressed = mReleased = false; }
	void bindToKey(Key key) { mKeyBinding = key; }

	bool isHeld() const { return mHeld; }
	bool isPressed() const { return mPressed; }
	bool isReleased() const { return mReleased; }

	bool isKey(SDL_Keycode key) const { return mKeyBinding == key; }

	void setHeld(bool isHeld) { mHeld = isHeld; }
	void setPressed(bool isPressed) { mPressed = isPressed; }
	void setReleased(bool isReleased) { mReleased = isReleased; }

	const Key key() const { return mKeyBinding; }

	int getHeldFrames() const { return mHeldFrames; }
	void incrementHeldFrame() { ++mHeldFrames; }
	void setHeldFrames(int heldFrames) { mHeldFrames = heldFrames; }


private:
	int mHeldFrames;
	bool mHeld;
	bool mPressed;
	bool mReleased;

	Key mKeyBinding;
};

