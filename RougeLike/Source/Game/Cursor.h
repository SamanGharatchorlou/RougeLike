#pragma once

class Texture;

class Cursor
{
public:
	Cursor() { }

	void setTexture(Texture* texture) { mTexture = texture; }

	void setSize(float x, float y) { mRect.SetSize(x, y); }

	void setPosition(float x, float y) { mRect.SetCenter(x, y); }
	VectorF getPosition() { return mRect.Center(); }


	// Inputs
	bool isMoving() const { return mMoving; }
	bool isHeld() const { return mHeld; }
	bool isPressed() const { return mPressed; }
	bool isReleased() const { return mReleased; }

	void setMotion(bool isMoving) { mMoving = isMoving; }
	void setHeld(bool isHeld) { mHeld = isHeld; }
	void setPressed(bool isPressed) { mPressed = isPressed; }
	void setReleased(bool isReleased) { mReleased = isReleased; }

	void render();

private:
	RectF mRect;
	Texture* mTexture;

	bool mMoving;
	bool mHeld;
	bool mPressed;
	bool mReleased;
};