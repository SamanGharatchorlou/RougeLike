#pragma once

#include "UIBox.h"
#include "Input/Button.h"


class Texture;

class UIButton : public UIBox
{
public:
	enum State
	{
		None,
		Hovering,
		Pressed,
		Active
	};


public:
	UIButton(const Attributes& attributes);
	~UIButton() { }

	void setTextures(Texture* defaultTexture, Texture* selected, Texture* hovering);

	void reset();

	void setState(State state);
	void setHeld(bool isHeld);
	void setPressed(bool isPressed);
	void setReleased(bool isReleased);

	inline bool isPressed() const { return mButton.isPressed(); }
	inline bool isReleased() const { return mButton.isReleased(); }
	inline bool isHeld() const { return mButton.isHeld(); }
	inline int holdCount() const { return mButton.getHeldFrames(); }

	virtual bool hasText() const { return false; }
	virtual Type type() const { return Type::Button; }

	void setActive(bool state);
	bool isActive() const { return mState == State::Active; }

private:
	Texture* mDefault;
	Texture* mSelected;
	Texture* mHovered;

	Button mButton;

	State mState;
};
