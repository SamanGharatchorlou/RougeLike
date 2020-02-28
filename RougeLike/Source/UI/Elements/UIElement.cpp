#include "pch.h"
#include "UIElement.h"

// TODO: for this to work I need to delay when the rect is set
// simple pass the x or x_P parameters some how?
// or just simplify it and only allow x_p to be set??
void UIElement::setRect(RectF rect)
{
	// WARNNING: this assumes rect is build based
	if (mParent != nullptr)
	{
		RectF parentRect = mParent->getRect();
		rect.x1 = parentRect.x1 + rect.x1;
		rect.y1 = parentRect.y1 + rect.y1;

		rect.SetWidth(rect.wi)

	}
}