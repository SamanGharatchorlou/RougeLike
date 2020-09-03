#include "pch.h"
#include "UIBox.h"

#include "Graphics/Texture.h"

#if DEBUG_CHECK
#include "Debug/DebugDraw.h"
#endif


UIBox::UIBox(Data& data) : UIElement(data), mTexture(data.texture)
{
#if DEBUG_CHECK
	mDrawRect = data.drawRect;
#endif
}


UIBox::UIBox() : mTexture(nullptr) 
{
#if DEBUG_CHECK
	mDrawRect = false;
#endif
}


UIBox::UIBox(const StringMap& attributes) : UIElement(attributes)
{
#if DEBUG_CHECK
	if (attributes.contains("debugDraw"))
		mDrawRect = true;
	else
		mDrawRect = false;
#endif
}

void UIBox::fill(const StringMap& attributes)
{
	UIElement::fill(attributes);
}


void UIBox::setTextureClipping()
{
	VectorF baseDimentions = mTexture->originalDimentions;
	VectorF requestedSize = mRect.Size();

	float baseRatio = baseDimentions.x / baseDimentions.y;
	float requestRatio = requestedSize.x / requestedSize.y;

	// wide
	if (baseRatio > 1.0f)
	{
		RectF textRect = RectF(VectorF(), baseDimentions);

		float newY = baseDimentions.y;
		float newX = newY * requestRatio;

		mSubrect.SetSize(VectorF(newX, newY));
		mSubrect.SetCenter(textRect.Center());
	}
}


void UIBox::render()
{
	if (mTexture)
	{
		if (mSubrect.Size().isPositive())
		{
			mTexture->renderSubTexture(mRect, mSubrect);
		}
		else
		{
			mTexture->render(mRect);
		}

	}

#if DEBUG_CHECK
	if (mDrawRect || DRAW_UI_RECTS)
		debugDrawRectOutline(mRect, RenderColour::Blue);
#endif
}


void UIBox::render(RectF subRect)
{
	if (mTexture)
		mTexture->renderSubTexture(mRect, subRect);

#if DEBUG_CHECK
	if (mDrawRect || DRAW_UI_RECTS)
		debugDrawRectOutline(mRect, RenderColour::Blue);
#endif
}


bool UIBox::isPointInBounds(VectorF point)
{
	bool xOverlaps = mRect.LeftPoint() < point.x && mRect.RightPoint() > point.x;
	bool yOverlaps = mRect.TopPoint() < point.y && mRect.BotPoint() > point.y;

	return xOverlaps && yOverlaps;
}