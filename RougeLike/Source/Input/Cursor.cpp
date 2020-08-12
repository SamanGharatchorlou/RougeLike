#include "pch.h"
#include "Cursor.h"
#include "Graphics/Texture.h"


Cursor::Cursor() : mTexture(nullptr), mMoving(false) { }


void Cursor::render()
{
	mTexture->render(mRect);
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
