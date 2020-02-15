#pragma once

class Texture;


class UIBox
{
public:
	UIBox(RectF rect, Texture* texture) : mRect(rect), mTexture(texture) { }

	bool isPointInBounds(VectorF point);

	void setTexture(Texture* texture) { mTexture = texture; }
	void render();

private:
	RectF mRect;
	Texture* mTexture;
};


class UIButton : public UIBox
{
public:
	enum ACTION 
	{
		NONE_0,
		RESUME_GAME_1,
		QUIT_GAME_2,

		COUNT
	};


public:
	UIButton(RectF rect, Texture* defaultTexture, Texture* selectedTexture, ACTION action);

	void setPressed(bool isPressed);
	void setReleased(bool isReleased);

	inline bool isPressed() const { return mIsPressed; }
	inline bool isReleased() const { return mIsReleased; }

	const ACTION getAction() const { return mAction; }

private:
	bool mIsPressed;
	bool mIsReleased;

	Texture* mDefaultTexture;
	Texture* mSelectedTexture;

	ACTION mAction;
};