#include "pch.h"
#include "UIButton.h"


UIButton::UIButton(const StringMap& attributes) :
	UIBox(attributes), mState(State::None),
	mDefault(nullptr), mSelected(nullptr), mHovered(nullptr) { }


void UIButton::setTextures(Texture* defaultTexture, Texture* selected, Texture* hovering)
{
	mDefault = defaultTexture;
	mSelected = selected;
	mHovered = hovering;
}


void UIButton::reset()
{
	mButton.setPressed(false);
	mButton.setReleased(false);
	mButton.setHeld(false);
	mButton.setHeldFrames(0);

	setState(State::None);
}


void UIButton::setState(State state)
{
	if (mState == State::Active)
		return;

	switch (state)
	{
	case UIButton::None:
		mTexture = mDefault;
		break;
	case UIButton::Hovering:
		mTexture = mHovered;
		break;
	case UIButton::Pressed:
	case UIButton::Active:
		mTexture = mSelected;
		break;
	default:
		DebugPrint(Warning, "Not a valid button state %d\n", state);
		break;
	}

	mState = state;
}


void UIButton::setActive(bool state)
{
	if (state)
	{
		setState(State::Active);
	}
	else
	{
		mState = State::None;
		mTexture = mDefault;
	}
}


void UIButton::setPressed(bool isPressed) 
{ 
	mButton.setPressed(isPressed);

	if (isPressed)
		setState(State::Pressed);
}


void UIButton::setHeld(bool isHeld)
{
	mButton.setHeld(isHeld);

	if (isHeld)
		setState(State::Pressed);
}


void UIButton::setReleased(bool isReleased) 
{ 
	mButton.setReleased(isReleased);

	if (isReleased)
		setState(State::Active);
}

