#include "pch.h"
#include "UIButton.h"


UIButton::UIButton(Data& data) :
	UIBox(data),
	mDefault(data.texture), mSelected(data.selectedTexture), mHovered(data.hoveringTexture),
	mState(State::None)
{
	ASSERT(Error, data.texture != nullptr, "UIButton must have a default texture assigned\n");
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


void UIButton::deactivate()
{
	mState = State::None;
	mTexture = mDefault;
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

