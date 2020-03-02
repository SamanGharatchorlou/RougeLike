#include "pch.h"
#include "UIElement.h"


UIElement::UIElement(Data& data) : mId(data.id), mParent(data.parent), mRect(data.rect) { }


// Rect must be set with relative coordinates
void UIElement::setRect(RectF rect)
{
	mRect = rect;
}


//void UIElement::rectToPixles(VectorF screenSize)
//{
//	RectF rect = mRect;
//
//	mRect.x1 = rect.x1 * screenSize.x;
//	mRect.SetWidth(rect.Width() * screenSize.x);
//
//	mRect.y1 = rect.y1 * screenSize.y;
//	mRect.SetHeight(rect.Height() * screenSize.y);
//}