#include "pch.h"
#include "Button.h"

Button::Button() : mKeyBinding(Key::None), mHeldFrames(0), mHeld(false), mPressed(false), mReleased(false) { }

Button::Button(Key key) : mKeyBinding(key), mHeldFrames(0), mHeld(false), mPressed(false), mReleased(false) { }

void Button::reset()
{
	mPressed = false;
	mReleased = false;

	if (mHeld)
		mHeldFrames++;
	else
		mHeldFrames = 0;
}