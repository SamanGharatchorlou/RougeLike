#pragma once

#include "UIBox.h"
#include "Input/Button.h"


class Texture;

class UIButton : public UIBox
{
public:
	struct Data : public UIBox::Data
	{
		Texture* highlightedTexture;
	};


public:
	UIButton();
	UIButton(Data& data);
	~UIButton() { }

	void reset();

	void setHeld(bool isHeld);
	void setPressed(bool isPressed);
	void setReleased(bool isReleased);

	inline bool isPressed() const { return mButton.isPressed(); }
	inline bool isReleased() const { return mButton.isReleased(); }
	inline bool isHeld() const { return mButton.isHeld(); }
	inline int holdCount() const { return mButton.getHeldFrames(); }

	virtual bool isButton() const { return true; }
	virtual bool hasText() const { return false; }
	virtual Type type() const { return Type::Button; }

	bool isActive() const { return mIsActive; }
	void setActive(bool state) { mIsActive = state; }

private:
	Texture* mDefault;
	Texture* mHighlighted;

	Button mButton;

	bool mIsActive;
};
