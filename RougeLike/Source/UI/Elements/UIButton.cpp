#include "pch.h"
#include "UIButton.h"

#include "Graphics/Texture.h"


UIButton::UIButton() :
	mDefault(nullptr), mHighlighted(nullptr)
{

}


UIButton::UIButton(Data& data) :
	UIBox(data),
	mDefault(data.texture), mHighlighted(data.highlightedTexture)
{
	ASSERT(Error, data.texture != nullptr, "UIButton must have a default texture assigned\n");
}


void UIButton::reset()
{
	mButton.setPressed(false);
	mButton.setReleased(false);
	mButton.setHeld(false);
	mButton.setHeldFrames(0);
}


void UIButton::setPressed(bool isPressed) 
{ 
	mButton.setPressed(isPressed);
	setTexture(isPressed ? mHighlighted : mDefault);
}


void UIButton::setHeld(bool isHeld)
{
	mButton.setHeld(isHeld);
}


void UIButton::setReleased(bool isReleased) 
{ 
	mButton.setReleased(isReleased);
}

