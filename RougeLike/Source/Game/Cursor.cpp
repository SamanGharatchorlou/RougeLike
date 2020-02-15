#include "pch.h"
#include "Cursor.h"
#include "Graphics/Texture.h"


void Cursor::render()
{
	mTexture->render(mRect);
}