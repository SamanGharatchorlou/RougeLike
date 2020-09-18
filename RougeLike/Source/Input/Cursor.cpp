#include "pch.h"
#include "Cursor.h"
#include "Graphics/Texture.h"

#if DEBUG_CURSOR
#include "Debug/DebugDraw.h"
#endif


Cursor::Cursor() : mTexture(nullptr), mMoving(false), mVisibility(false), mMode(Cursor::None) { }


void Cursor::render()
{
#if DEBUG_CURSOR
	debugDrawRect(mRect, RenderColour::LightGrey);
#endif

	mTexture->render(mRect);

#if DEBUG_CURSOR // ERROR: Needs to be converted to camera coords
	debugDrawPoint(position(), 5.0f, RenderColour::Black);
#endif
}

void Cursor::setButton(ButtonType type, Button button)
{
	mButtons[type] = button;
}

void Cursor::clearInputs()
{
	mButtons[Cursor::Left].setPressed(false);
	mButtons[Cursor::Right].setPressed(false);

	mButtons[Cursor::Left].setReleased(false);
	mButtons[Cursor::Right].setReleased(false);
}


void Cursor::setCursorOffsetPoint(VectorF offset)
{
	mCursorOffsetPoint = offset;
}


// Query Input
bool Cursor::isHeld(ButtonType type) const
{
	return mButtons[type].isHeld();
}

bool Cursor::isPressed(ButtonType type) const
{
	return mButtons[type].isPressed();
}

bool Cursor::isReleased(ButtonType type) const
{
	return mButtons[type].isReleased();
}
