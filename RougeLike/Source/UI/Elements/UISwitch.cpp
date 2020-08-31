#include "pch.h"
#include "UISwitch.h"

#include "Input/InputManager.h"



UISwitch::UISwitch(const StringMap& attributes) :
	UIBox(attributes), mState(State::Off),
	mOn(nullptr), mOff(nullptr) { }


void UISwitch::setTextures(Texture* off, Texture* on)
{
	mOff = off;
	mOn = on;

	mTexture = mOff;
}


void UISwitch::handleInput(const InputManager* input)
{
	if (isPointInBounds(input->cursorPosition()))
	{
		if (input->isCursorPressed(Cursor::Left))
			switchState();
	}
	else
	{
		reset();
	}
}


void UISwitch::setState(State state)
{
	mState = state;

	if (mState == Off)
		mTexture = mOff;
	else if (state == On)
		mTexture = mOn;
}


void UISwitch::switchState()
{
	if (mState == On)
	{
		mState = Off;
		mTexture = mOff;
	}
	else if (mState == Off)
	{
		mState = On;
		mTexture = mOn;
	}
	else
	{
		DebugPrint(Warning, "Current state not valid, switch cannot change state\n");
	}
}