#include "pch.h"
#include "UIBox.h"

#include "Graphics/Texture.h"


UIBox::UIBox(Data& data) : UIElement(data), mTexture(data.texture) { }


void UIBox::render()
{
	if(mTexture)
		mTexture->render(mRect);
}


bool UIBox::isPointInBounds(VectorF point)
{
	bool xOverlaps = mRect.LeftPoint() < point.x && mRect.RightPoint() > point.x;
	bool yOverlaps = mRect.TopPoint() < point.y && mRect.BotPoint() > point.y;

	return xOverlaps && yOverlaps;
}