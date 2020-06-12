#include "pch.h"
#include "Camera.h"
#include "Map/Map.h"


Camera* Camera::Get()
{
	static Camera sInstance;
	return &sInstance;
}


Camera::Camera() : mFollowingRect(nullptr)
{
	mActiveRect = &mRect;
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


void Camera::fastUpdate(float dt)
{
	VectorF translation = mFollowingRect->Center() - mRect.Center();

	if (mRect.LeftPoint() + translation.x >= mBoundaries.x1 &&
		mRect.RightPoint() + translation.x <= mBoundaries.x2)
	{
		mRect = mRect.Translate(translation.x, 0.0f);
	}
	else
	{
		int a = 4;
	}

	if (mRect.TopPoint() + translation.y >= mBoundaries.y1 &&
		mRect.BotPoint() + translation.y <= mBoundaries.y2)
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


VectorF Camera::toCameraCoords(const VectorF worldCoords) const
{
	return worldCoords - mActiveRect->TopLeft();
}
