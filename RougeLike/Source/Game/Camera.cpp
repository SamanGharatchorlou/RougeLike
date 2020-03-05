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

void Camera::setupCamera(Map* map)
{
	// TODO: not exactly center, maybe its top left of the center tile? or theres not a center tile?
	VectorF startingPosition(0.0f, map->size().y / 2);
	ASSERT(Warning, !mRect.Size().isZero(), "Viewport has not been setup\n");

	mRect.SetLeftCenter(startingPosition);

	mActiveRect = &mRect;

	boundaries = map->size();	

	// TEMP - shake setup
	shakeyCam.init(150, 200, 20.0f);
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
		mRect.RightPoint() + translation.x <= boundaries.x)
	{
		mRect = mRect.Translate(translation.x, 0.0f);
	}

	if (mRect.TopPoint() + translation.y >= 0.0f &&
		mRect.BotPoint() + translation.y <= boundaries.y)
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

VectorF Camera::toWorldCoords(VectorF cameraCoords)
{
	return cameraCoords - mActiveRect->TopLeft();
}

RectF Camera::toWorldCoords(RectF cameraRect)
{
	return RectF(cameraRect.TopLeft() - mActiveRect->TopLeft(), cameraRect.Size());
}