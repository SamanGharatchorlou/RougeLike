#include "pch.h"
#include "Camera.h"

#include "System/Window.h"
#include "Map/Map.h"
#include "Characters/Attributes/Movement.h"


void Camera::setupCamera(Map* map)
{
	// TODO: not exactly center, maybe its top left of the center tile? or theres not a center tile?
	VectorF startingPosition(0.0f, map->size().y / 2);
	ASSERT(Warning, !mRect.Size().isZero(), "Viewport has not been setup\n");

	mRect.SetLeftCenter(startingPosition);

	boundaries = map->size();	
}


void Camera::follow(RectF* rect)
{
	mFollowingRect = rect;
}


void Camera::update()
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


VectorF Camera::toCameraCoords(VectorF worldCoords)
{
	return worldCoords - mRect.TopLeft();
}

RectF Camera::toCameraCoords(RectF worldRect)
{
	return RectF(worldRect.TopLeft() - mRect.TopLeft(), worldRect.Size());
}

Rect<int> Camera::toCameraCoords(Rect<int> worldRect)
{
	return Rect<int>(worldRect.TopLeft() - mRect.TopLeft().toInt(), worldRect.Size());
}

VectorF Camera::toWorldCoords(VectorF cameraCoords)
{
	return cameraCoords - mRect.TopLeft();
}

RectF Camera::toWorldCoords(RectF cameraRect)
{
	return RectF(cameraRect.TopLeft() - mRect.TopLeft(), cameraRect.Size());
}