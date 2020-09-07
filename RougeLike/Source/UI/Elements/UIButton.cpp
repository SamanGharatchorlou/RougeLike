#include "pch.h"
#include "UIButton.h"

#include "Input/InputManager.h"

UIButton::UIButton(const StringMap& attributes) :
	UIBox(attributes), mIsActive(false),
	mDefault(nullptr), mSelected(nullptr), mHovered(nullptr) { }


UIButton::UIButton(Data& buttonData) : UIBox(buttonData), mIsActive(false)
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


bool UIButton::handleInput(const InputManager* input)
{
	bool hasPressed = false;
	if (isPointInBounds(input->cursorPosition()))
	{
		setState(UIButton::State::Hovering);
		setHeld(input->isCursorHeld(Cursor::Left));
		setReleased(input->isCursorReleased(Cursor::Left));

		if (input->isCursorPressed(Cursor::Left))
		{
			setPressed(true);
			hasPressed = true;
		}
	}
	else
	{
		reset();
	}

	return hasPressed;
}


void UIButton::reset()
{
	mButton.setPressed(false);
	mButton.setReleased(false);
	mButton.setHeld(false);
	mButton.setHeldFrames(0);

	setActive(false);
	setState(State::None);
}


void UIButton::setState(State state)
{
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

