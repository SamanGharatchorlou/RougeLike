#include "pch.h"
#include "UIElement.h"


UIElement::UIElement(Data& data) : mId(data.id), mParent(data.parent)
{
	setRect(data.rect);
}


// Rect must be set with relative coordinates
void UIElement::setRect(RectF rect)
{
	// WARNNING: this assumes rect is build based
	if (mParent != nullptr)
	{
		// UI TODO: at this point of calculation the parent rect still has relative positions
		// find a way to calcualte the relative position of this element using the parent relative positon
		RectF parentRect = mParent->getRect();
		rect.x1 = parentRect.x1 + rect.x1;
		rect.y1 = parentRect.y1 + rect.y1;

		rect.SetWidth(rect.Width() * parentRect.Width());
		rect.SetHeight(rect.Height() * parentRect.Height());
	}

	mRect = rect;
}


void UIElement::rectToPixles(VectorF screenSize)
{
	RectF rect = mRect;

	mRect.x1 = rect.x1 * screenSize.x;
	mRect.SetWidth(rect.Width() * screenSize.x);

	mRect.y1 = rect.y1 * screenSize.y;
	mRect.SetHeight(rect.Height() * screenSize.y);
}