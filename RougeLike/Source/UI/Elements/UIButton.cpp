#include "pch.h"
#include "UIButton.h"

#include "Graphics/Texture.h"


UIButton::UIButton() :
	mDefault(nullptr), mHighlighted(nullptr),
	mAction(None_0),
	mIsPressed(false), mIsReleased(false)
{

}


UIButton::UIButton(Data& data) :
	UIBox(data),
	mDefault(data.texture), mHighlighted(data.highlightedTexture),
	mAction(data.action),
	mIsPressed(false), mIsReleased(false)
{
	ASSERT(Error, data.texture != nullptr, "UIButton must have a default texture assigned\n");
}


void UIButton::setPressed(bool isPressed) 
{ 
	mIsPressed = isPressed; 

	setTexture(isPressed ? mHighlighted : mDefault);
}


void UIButton::setReleased(bool isReleased) 
{ 
	mIsReleased = isReleased; 
	setTexture(isReleased ? mDefault : mHighlighted);
}



// -- UI Text Button -- //
UITextButton::UITextButton(Data& data) : 
	mText(data.text, data.font, data.ptSize, data.colour),
	UIButton(data)
{

}


void UITextButton::render()
{
	UIButton::render();
	mText.render(mRect.TopLeft());
}