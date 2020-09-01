#pragma once

#if _DEBUG
#include "Debug/DebugDraw.h"
#endif


class Button
{
public:
	enum class State
	{
		None,
		Pressed,
		Released,
		Held
	};

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
		Space = SDLK_SPACE,

		// Keys
		E = SDLK_e,

		// Numbers
		Zero = SDLK_0,
		One = SDLK_1,
		Two = SDLK_2,
		Three = SDLK_3,
		Four = SDLK_4,
		Five = SDLK_5,
		Six = SDLK_6,
		Seven = SDLK_7,
		Eight = SDLK_8,
		Nine = SDLK_9
	};


public:
	Button();
	Button(Key key);
	void bind(Key key) { mKeyBinding = key; }

	void reset();

	State state() const;

	bool isHeld() const { return mHeld; }
	bool isPressed() const { return mPressed; }
	bool isReleased() const { return mReleased; }

	const Key key() const { return mKeyBinding; }
	bool isKey(SDL_Keycode key) const { return mKeyBinding == key; }
	bool isKey(Key key) const { return mKeyBinding == key; }

	void setHeld(bool isHeld) { mHeld = isHeld; }
	void setPressed(bool isPressed) { mPressed = isPressed; }
	void setReleased(bool isReleased) { mReleased = isReleased; }

	int getHeldFrames() const { return mHeldFrames; }
	void setHeldFrames(int frames) { mHeldFrames = frames; }
	void incrementHeldFrames() { mHeldFrames++; }

private:
	int mHeldFrames;
	bool mHeld;
	bool mPressed;
	bool mReleased;

	Key mKeyBinding;
};

