#include "pch.h"
#include "Camera.h"
#include "Map/Map.h"


Camera* Camera::Get()
{
	static Camera sInstance;
	return &sInstance;
}


void Camera::clear()
{
	mActiveRect = nullptr;
	mFollowingRect = nullptr;
	shakeyCam.clear();
}


Camera::Camera() : mFollowingRect(nullptr), mScale(1.0f)
{
	mActiveRect = &mRect;
}


void Camera::setScale(float scale)
{
	mScale = scale;
	setViewport(size() / mScale);
}


void Camera::setPosition(VectorF position)
{
	mRect.SetLeftCenter(position);
	mActiveRect = &mRect;
}


void Camera::follow(RectF* rect)
{
	mFollowingRect = rect;
	mRect.SetCenter(mFollowingRect->Center());
}


void Camera::fastUpdate(float dt)
{
	VectorF translation = lerpMovement(dt);

#if !CAMERA_IGNORE_BOUNDARIES
	if (mRect.LeftPoint() + translation.x >= mBoundaries.x1 &&
		mRect.RightPoint() + translation.x <= mBoundaries.x2)
#endif
	{
		mRect = mRect.Translate(translation.x, 0.0f);
	}


#if !CAMERA_IGNORE_BOUNDARIES
	if (mRect.TopPoint() + translation.y >= mBoundaries.y1 &&
		mRect.BotPoint() + translation.y <= mBoundaries.y2)
#endif
	{
		mRect = mRect.Translate(0.0f, translation.y);
	}


	if (shakeyCam.hasTrauma())
	{
		shakeyCam.enable(mRect, mBoundaries);
		shakeyCam.fastUpdate(dt);
	}
}



void Camera::slowUpdate(float dt)
{
	if (shakeyCam.hasTrauma())
	{
		mActiveRect = shakeyCam.rect();

#if PRINT_SHAKEYCAM_VALUES
		DebugPrint(Log, "Trauma %.3f | Offset = %.3f, %.3f\n",
			shakeyCam.trauma(),
			shakeyCam.offset().x, shakeyCam.offset().y);
#endif
	}
	else
	{
		mActiveRect = &mRect;
	}


}


VectorF Camera::toCameraCoords(const VectorF& worldCoords) const
{
	return worldCoords - mActiveRect->TopLeft();
}


Quad2D<float> Camera::toCameraCoords(const Quad2D<float>& worldCoords) const
{
	VectorF translation = toCameraCoords(worldCoords.at(0)) - worldCoords.at(0);
	Quad2D<float> newQuad(worldCoords);
	newQuad.translate(translation);

	return newQuad;
}


VectorF Camera::lerpMovement(float dt)
{
	return (mFollowingRect->Center() - mRect.Center()) * 0.9f * dt * 10.0f;
}