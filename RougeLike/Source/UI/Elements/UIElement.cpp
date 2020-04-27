#include "pch.h"
#include "UIElement.h"


UIElement::UIElement(Data& data) : mId(data.id), mParent(data.parent), mRect(data.rect) { }


// Rect must be set with relative coordinates
void UIElement::setRect(RectF rect)
{
	mRect = rect;
}