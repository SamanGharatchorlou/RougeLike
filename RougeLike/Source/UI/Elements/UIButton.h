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

	struct Data : public UIBox::Data
	{
		Texture* defaultTexture = nullptr;
		Texture* selected = nullptr;
		Texture* hovered = nullptr;
	};


public:
	UIButton(const StringMap& attributes);
	UIButton(Data& data);
	~UIButton() { }

	void setTextures(Texture* defaultTexture, Texture* selected, Texture* hovering);

	void reset();

	bool handleInput(const InputManager* input);

	void setState(State state);
	void setHeld(bool isHeld);
	void setPressed(bool isPressed);
	void setReleased(bool isReleased);

	inline bool isPressed() const { return mButton.isPressed(); }
	inline bool isReleased() const { return mButton.isReleased(); }
	inline bool isHeld() const { return mButton.isHeld(); }
	inline int holdCount() const { return mButton.getHeldFrames(); }

	virtual Type type() const override { return Type::Button; }

	void setActive(bool active) { mIsActive = active; }
	bool isActive() const { return mIsActive; }

private:
	Texture* mDefault;
	Texture* mSelected;
	Texture* mHovered;

	Button mButton;

	bool mIsActive;
};
