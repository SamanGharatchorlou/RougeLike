#include "pch.h"
#include "UIBox.h"

#include "Graphics/Texture.h"

#if _DEBUG
#include "Debug/DebugDraw.h"
#endif


UIBox::UIBox(Data& data) : UIElement(data), mTexture(data.texture)
{
#if _DEBUG
	mDrawRect = data.drawRect;
#endif
}


UIBox::UIBox() : mTexture(nullptr) 
{
#if _DEBUG
	mDrawRect = false;
#endif
}


UIBox::UIBox(const StringMap& attributes) : UIElement(attributes)
{
#if _DEBUG
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


void UIBox::render()
{
	if(mTexture)
		mTexture->render(mRect);

#if _DEBUG
	if (mDrawRect || DRAW_UI_RECTS)
		debugDrawRectOutline(mRect, RenderColour::Blue);
#endif
}


void UIBox::render(RectF subRect)
{
	if (mTexture)
		mTexture->renderSubTexture(mRect, subRect);

#if _DEBUG
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