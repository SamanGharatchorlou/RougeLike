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

public:
	Cursor();
	void render();

	void setTexture(Texture* texture) { mTexture = texture; }
	void setSize(VectorF size) { mRect.SetSize(size); }

	void setPosition(float x, float y) { mRect.SetCenter(x, y); }
	VectorF position() const { return mRect.Center(); }

	Button getButton(ButtonType type) const { return mButtons[type]; }
	void setButton(ButtonType type, Button button);
	void setMotion(bool isMoving) { mMoving = isMoving; }

	void clearInputs();

	// Inputs
	bool isMoving() const { return mMoving; }
	bool isHeld(ButtonType type = ButtonType::Left) const;	
	bool isPressed(ButtonType type = ButtonType::Left) const;
	bool isReleased(ButtonType type = ButtonType::Left) const;

public:
	RectF mRect;
	Texture* mTexture;

	bool mMoving;
	Button mButtons[2]; 
};