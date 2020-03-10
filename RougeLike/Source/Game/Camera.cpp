#include "pch.h"
#include "Camera.h"

#include "System/Window.h"
#include "Map/Map.h"
#include "Characters/Attributes/Movement.h"

// TEMP
Camera* Camera::Get()
{
	static Camera sInstance;
	return &sInstance;
}


void Camera::setPosition(VectorF position)
{
	mRect.SetLeftCenter(position);
	mActiveRect = &mRect;
}


void Camera::follow(RectF* rect)
{
	mFollowingRect = rect;
}


// TODO: does this need to be in fast update? could be slow maybe?
void Camera::fastUpdate(float dt)
{
	VectorF translation = mFollowingRect->Center() - mRect.Center();

	if (mRect.LeftPoint() + translation.x >= 0.0f &&
		mRect.RightPoint() + translation.x <= mBoundaries.x)
	{
		mRect = mRect.Translate(translation.x, 0.0f);
	}

	if (mRect.TopPoint() + translation.y >= 0.0f &&
		mRect.BotPoint() + translation.y <= mBoundaries.y)
	{
		mRect = mRect.Translate(0.0f, translation.y);
	}
}



void Camera::slowUpdate(float dt)
{
	if (shakeyCam.hasTrauma())
	{
		mActiveRect = shakeyCam.rect();

		shakeyCam.setRect(mRect);
		shakeyCam.update(dt);
	}
	else
	{
		mActiveRect = &mRect;
	}
}


VectorF Camera::toCameraCoords(VectorF worldCoords)
{
	return worldCoords - mActiveRect->TopLeft();
}

RectF Camera::toCameraCoords(RectF worldRect)
{
	return RectF(worldRect.TopLeft() - mActiveRect->TopLeft(), worldRect.Size());
}

Rect<int> Camera::toCameraCoords(Rect<int> worldRect)
{
	return Rect<int>(worldRect.TopLeft() - mActiveRect->TopLeft().toInt(), worldRect.Size());
}
