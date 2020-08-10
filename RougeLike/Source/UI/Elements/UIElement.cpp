#include "pch.h"
#include "UIElement.h"


UIElement::UIElement(const StringMap& attributes) 
{
	fill(attributes);
}


void UIElement::fill(const StringMap& attributes)
{
	float x = attributes.getFloat("x");
	float y = attributes.getFloat("y");

	float width = attributes.getFloat("width");
	float height = attributes.getFloat("height");

	mRect = RectF(VectorF(x, y), VectorF(width, height));

	if(attributes.contains("id"))
		mId = attributes.getString("id");
}


void UIElement::setRect(RectF rect)
{
	mRect = rect;
}