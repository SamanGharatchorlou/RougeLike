#pragma once

#include "Input/Button.h"

class Texture;

class Cursor
{
public:
	enum ButtonType
	{
		Left,
		Right,
		Count
	};

	enum Mode
	{
		None,
		Menu,
		Game_Red,
		Game_Green
	};

public:
	Cursor();
	void render();

	void setTexture(Texture* texture) { mTexture = texture; }

	Mode mode() const { return mMode; }
	void setMode(Mode mode) { mMode = mode; }

	VectorF size() const { return mRect.Size(); }
	void setSize(VectorF size) { mRect.SetSize(size); }

	void setPosition(VectorF position) { mRect.SetCenter(position); }
	VectorF position() const { return mRect.Center() + mCursorOffsetPoint; }

	void setCursorOffsetPoint(VectorF offset);

	void setVisibility(bool isVisible) { mVisibility = isVisible; }
	bool isVisible() const { return mVisibility; }

	Button getButton(ButtonType type) const { return mButtons[type]; }
	void setButton(ButtonType type, Button button);
	void setMotion(bool isMoving) { mMoving = isMoving; }

	void clearInputs();

	// Inputs
	bool isMoving() const { return mMoving; }
	bool isHeld(ButtonType type = ButtonType::Left) const;	
	bool isPressed(ButtonType type = ButtonType::Left) const;
	bool isReleased(ButtonType type = ButtonType::Left) const;


private:
	RectF mRect;
	Texture* mTexture;

	VectorF mCursorOffsetPoint;

	bool mMoving;
	Button mButtons[2];

	bool mVisibility;
	Mode mMode;
};