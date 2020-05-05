#include "pch.h"
#include "Button.h"

void Button::reset()
{
	mPressed = false;
	mReleased = false;

	if (mHeld)
		mHeldFrames++;
	else
		mHeldFrames = 0;
}