#include "pch.h"
#include "UISlider.h"

#include "Input/InputManager.h"

#include "UIButton.h"
#include "Graphics/Texture.h"


UISlider::UISlider(const StringMap& attributes) : UIBox(attributes), xCursorOffset(0.0f), mSlider(nullptr), mBar(nullptr), mSelectable(true) { }

UISlider::~UISlider()
{
	if (mSlider)
	{
		delete mSlider;
		mSlider = nullptr;
	}

	if (mBar)
	{
		delete mBar;
		mBar = nullptr;
	}
}


void UISlider::setComponents(Texture* container, UIButton* slider, UIBox* bar)
{
	mTexture = container;

	mBar = bar;
	mBarSubRect = defaultBarSubRect();
	mBar->setRect(defaultBarRect());

	mSlider = slider;
	VectorF sliderSize = realiseSize(mSlider->texture()->originalDimentions, mRect.Height() * 1.35f);
	RectF sliderRect = RectF(VectorF(), sliderSize);
	sliderRect.SetCenter(mRect.Center());
	mSlider->setRect(sliderRect);

	setSliderValue(1.0f);
}



void UISlider::handleInput(const InputManager* input)
{
	if (!mSelectable)
	{
		mSlider->reset();
		return;
	}

	if (mSlider->isPointInBounds(input->cursorPosition()))
	{
		mSlider->setState(UIButton::State::Hovering);
		mSlider->setPressed(input->isCursorPressed(Cursor::Left));
		mSlider->setHeld(input->isCursorHeld(Cursor::Left));
		mSlider->setReleased(input->isCursorReleased(Cursor::Left));
	}
	else
	{
		mSlider->reset();
	}


	// Activate slider
	if (mSlider->isPressed())
	{
		mSlider->setActive(true);
		setCursorOffset(input->cursorPosition().x);
	}

	// SetSlider position
	if (mSlider->isActive())
	{
		setSliderPosition(input->cursorPosition().x);
	}

	// Deactivate slider
	if (input->isCursorReleased(Cursor::Left))
	{
		mSlider->setActive(false);
	}
}


void UISlider::setCursorOffset(float cursorPosition)
{
	xCursorOffset = mSlider->rect().Center().x - cursorPosition;
}


void UISlider::setSliderPosition(float position)
{
	position = clamp(position + xCursorOffset, mRect.LeftPoint(), mRect.RightPoint());

	// Move slider
	RectF sliderRect = mSlider->rect();
	sliderRect.SetCenter(VectorF(position, sliderRect.Center().y));
	mSlider->setRect(sliderRect);

	updateBar();
}


void UISlider::setSliderValue(float value)
{
	value = clamp(value, 0.0f, 1.0f);
	float width = value * mRect.Width();

	// Move slider
	float position = mRect.LeftPoint() + width;
	setSliderPosition(position);

	updateBar();
}


float UISlider::getValue() const
{
	float width = mSlider->rect().Center().x + xCursorOffset - mRect.LeftPoint();
	float boxWidth = mRect.Width();
	return width / boxWidth;
}


void UISlider::render()
{
	if (mTexture)
		mTexture->render(mRect);

	mBar->render(mBarSubRect);
	mSlider->render();


#if _DEBUG
	if (mDrawRect || DRAW_UI_RECTS)
		debugDrawRectOutline(mRect, RenderColour::Blue);
#endif
}


// -- Private Functions -- //
void UISlider::updateBar()
{
	float value = getValue();
	setBarSubRect(value);
	setBarRect(value);
}


void UISlider::setBarRect(float value)
{
	RectF barRect = defaultBarRect();
	barRect.SetSize(barRect.Size() * VectorF(value, 1.0f));
	mBar->setRect(barRect);
}


void UISlider::setBarSubRect(float value)
{
	mBarSubRect = defaultBarSubRect();
	mBarSubRect.SetWidth(mBarSubRect.Width() * value);
}


RectF UISlider::defaultBarSubRect() const
{
	RectF originalRect = RectF(VectorF(), mBar->texture()->originalDimentions);
	RectF innerBarRect = RectF(VectorF(), mBar->texture()->originalDimentions * VectorF(1.0f, 0.8f));
	innerBarRect.SetCenter(originalRect.Center());
	return innerBarRect;
}


RectF UISlider::defaultBarRect() const
{
	VectorF size = mRect.Size() * VectorF(0.975f, 0.7f);
	RectF barRect = RectF(VectorF(), size);
	barRect.SetCenter(mRect.Center());
	return barRect;
}