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