#pragma once

class Button
{
public:

	enum Key
	{
		None = 0,

		// Movement
		Up = SDLK_w,
		Down = SDLK_s,
		Left = SDLK_a,
		Right = SDLK_d,

		// Game state
		Pause = SDLK_p,
		Esc = SDLK_ESCAPE,
		Quit = SDL_QUIT,

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

	const Key getKey() const { return mKeyBinding; }

	int getHeldFrames() const { return mHeldFrames; }
	void incrementHeldFrame() { ++mHeldFrames; }


private:
	int mHeldFrames;
	bool mHeld;
	bool mPressed;
	bool mReleased;

	Key mKeyBinding;
};

