#include "pch.h"
#include "UIButton.h"

#include "Input/InputManager.h"

UIButton::UIButton(const StringMap& attributes) :
	UIBox(attributes), mState(State::None),
	mDefault(nullptr), mSelected(nullptr), mHovered(nullptr) { }


UIButton::UIButton(Data& buttonData) : UIBox(buttonData)
{
	setTextures(buttonData.defaultTexture, buttonData.selected, buttonData.hovered);
}


void UIButton::setTextures(Texture* defaultTexture, Texture* selected, Texture* hovering)
{
	mDefault = defaultTexture;

	mSelected = selected != nullptr ? selected : defaultTexture;
	mHovered = hovering != nullptr ? hovering : defaultTexture;

	mTexture = mDefault;
}


void UIButton::handleInput(const InputManager* input)
{
	if (isPointInBounds(input->cursorPosition()))
	{
		setState(UIButton::State::Hovering);
		setPressed(input->isCursorPressed(Cursor::Left));
		setHeld(input->isCursorHeld(Cursor::Left));
		setReleased(input->isCursorReleased(Cursor::Left));
	}
	else
	{
		reset();
	}
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

