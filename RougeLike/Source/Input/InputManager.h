#pragma once

class Cursor;

static constexpr int maxButtons = 10;

class Button
{
public:

	enum KEY
	{
		NONE = 0,

		// Movement
		UP = SDLK_w,
		DOWN = SDLK_s,
		LEFT = SDLK_a,
		RIGHT = SDLK_d,

		// Game state
		PAUSE = SDLK_p,
		ESC = SDLK_ESCAPE,
		QUIT = SDL_QUIT
	};


public:
	Button() { mHeldFrames = 0; mHeld = mPressed = mReleased = false; }
	void bindToKey(KEY key) { mKeyBinding = key; }

	bool isHeld() const { return mHeld; }
	bool isPressed() const { return mPressed; }
	bool isReleased() const { return mReleased; }

	bool isKey(SDL_Keycode key) const { return mKeyBinding == key; }

	void setHeld(bool isHeld) { mHeld = isHeld; }
	void setPressed(bool isPressed) { mPressed = isPressed; }
	void setReleased(bool isReleased) { mReleased = isReleased; }

	KEY getKey() { return mKeyBinding; }

	int getHeldFrames() const { return mHeldFrames; }
	void incrementHeldFrame() { ++mHeldFrames; }

private:
	int mHeldFrames;
	bool mHeld;
	bool mPressed;
	bool mReleased;

	KEY mKeyBinding;
};



class InputManager
{
public:
	InputManager(Cursor* cursor) : mCursor(cursor) { }
	~InputManager() { }

	void init();
	void processInputEvent(SDL_Event event);
	void update();

	// Buttons
	const Button& getButton(Button::KEY key);

	bool isHeld(Button::KEY key) { return getButton(key).isHeld(); }
	bool isPressed(Button::KEY key) { return getButton(key).isPressed(); }
	bool isReleased(Button::KEY key) { return getButton(key).isReleased(); }

	int getHeldFrames(Button::KEY key) { return getButton(key).getHeldFrames(); }

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