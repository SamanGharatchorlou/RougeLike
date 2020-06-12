#include "pch.h"
#include "UISlider.h"

#include "UI/Elements/UIButton.h"

UISlider::UISlider() : mSlider(nullptr), mBar(nullptr), mContainer(nullptr) { }


void UISlider::setComponents(UIButton* slider, UIElement* bar, UIElement* container)
{
	mSlider = slider;
	mBar = bar;
	mContainer = container;
}


bool UISlider::isActive() const
{
	return mSlider->isActive();
}


void UISlider::setActive(bool active)
{
	mSlider->setActive(active);
}


bool UISlider::isPressed() const
{
	return mSlider->isPressed();
}


void UISlider::setCursorOffset(float cursorPosition)
{
	xCursorOffset = mSlider->rect().Center().x - cursorPosition;
}


void UISlider::setPosition(float position)
{
	position = clamp(position, mContainer->rect().LeftPoint() + 10.0f, mContainer->rect().RightPoint() - 8.0f);

	// Move slider
	RectF sliderRect = mSlider->rect();
	sliderRect.SetCenter(VectorF(position + xCursorOffset, sliderRect.Center().y));
	mSlider->setRect(sliderRect);

	// Adjust bar width
	float width = position + xCursorOffset - mContainer->rect().LeftPoint();
	RectF barRect = mBar->rect();
	barRect.SetSize(width, barRect.Height());
	mBar->setRect(barRect);
}


void UISlider::setValue(float value)
{
	value = clamp(value, 0.0f, 1.0f);
	float width = value * mContainer->rect().Width();

	// Adjust bar width
	RectF barRect = mBar->rect();
	barRect.SetSize(width, barRect.Height());
	mBar->setRect(barRect);

	// Move slider
	float position = mContainer->rect().LeftPoint() + width;
	RectF sliderRect = mSlider->rect();
	sliderRect.SetCenter(VectorF(position, sliderRect.Center().y));
	mSlider->setRect(sliderRect);
}


float UISlider::value() const
{
	float maxWidth = mContainer->rect().Width();
	float currentWidth = mBar->rect().Width();

	return currentWidth / maxWidth;
}