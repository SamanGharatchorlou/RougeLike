#include "pch.h"
#include "UIButton.h"

#include "Graphics/Texture.h"


void UIBox::render()
{
	mTexture->render(mRect);
}


bool UIBox::isPointInBounds(VectorF point)
{
	bool xOverlaps = mRect.LeftPoint() < point.x && mRect.RightPoint() > point.x;
	bool yOverlaps = mRect.TopPoint() < point.y && mRect.BotPoint() > point.y;

	return xOverlaps && yOverlaps;
}


UIButton::UIButton(RectF rect, Texture* defaultTexture, Texture* selectedTexture, ACTION action) :
	UIBox(rect, defaultTexture),
	mDefaultTexture(defaultTexture),
	mAction(action),
	mIsPressed(false),
	mIsReleased(false)
{
	mSelectedTexture = (selectedTexture == nullptr) ? defaultTexture : selectedTexture;
}


void UIButton::setPressed(bool isPressed) 
{ 
	mIsPressed = isPressed; 

	if (isPressed)
	{
		printf("button pressed!\n");
	}

	if (isPressed)
		setTexture(mSelectedTexture);
	else
		setTexture(mDefaultTexture);
}


void UIButton::setReleased(bool isReleased) 
{ 
	mIsReleased = mIsReleased; 

	if (isReleased)
		setTexture(mDefaultTexture);
	else
		setTexture(mSelectedTexture);
}