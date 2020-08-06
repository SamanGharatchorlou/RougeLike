#include "pch.h"
#include "UIElement.h"


UIElement::UIElement(const Attributes& attributes) 
{
	float x = attributes.getFloat("x");
	float y = attributes.getFloat("y");

	float width = attributes.getFloat("width");
	float height = attributes.getFloat("height");

	mRect = RectF(VectorF(x, y), VectorF(width, height));
	mId = attributes.getString("id");
}


void UIElement::setRect(RectF rect)
{
	mRect = rect;
}