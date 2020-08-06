#include "pch.h"
#include "UIBox.h"

#include "Graphics/Texture.h"

#if _DEBUG
#include "Debug/DebugDraw.h"
#endif

UIBox::UIBox() : mTexture(nullptr) 
{
#if _DEBUG
	mDrawRect = false;
#endif
}


UIBox::UIBox(const Attributes& attributes) : UIElement(attributes)
{
#if _DEBUG
	if (attributes.contains("debugDraw"))
		mDrawRect = true;
	else
		mDrawRect = false;
#endif
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


bool UIBox::isPointInBounds(VectorF point)
{
	bool xOverlaps = mRect.LeftPoint() < point.x && mRect.RightPoint() > point.x;
	bool yOverlaps = mRect.TopPoint() < point.y && mRect.BotPoint() > point.y;

	return xOverlaps && yOverlaps;
}