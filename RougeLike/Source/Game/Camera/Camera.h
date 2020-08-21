#pragma once

#include "CameraShake.h"


class Camera
{

public:
	static Camera* Get();

	void setViewport(VectorF viewport) { mRect.SetSize(viewport); }
	void setPosition(VectorF position);

	void setMapBoundaries(RectF boundaries) { mBoundaries = boundaries; }

	// follow this object
	void follow(RectF* rect);

	void slowUpdate(float dt);
	void fastUpdate(float dt);

	template <typename T>
	bool inView(Rect<T> object) const;

	template <typename T>
	bool inView(Vector2D<T> point) const;

	template <typename T>
	Rect<T> toCameraCoords(const Rect<T> worldCoords) const;
	VectorF toCameraCoords(const VectorF worldCoords) const;

	RectF rect() const { return mRect; }
	VectorF size() const { return mRect.Size(); }

	void initShakeyCam(float maxTrauma, float traumaReduction, float maxAngle) { shakeyCam.init(maxTrauma, traumaReduction, maxAngle); }
	CameraShake* getShake() { return &shakeyCam; }


private:
	Camera();
	~Camera() { }

	VectorF lerpMovement(float dt);


private:
	RectF mRect;
	RectF mBoundaries;
	const RectF* mActiveRect;
	RectF* mFollowingRect;

	CameraShake shakeyCam;
};


template <typename T>
bool Camera::inView(Rect<T> object) const
{
	if (object.RightPoint() < (T)mRect.LeftPoint()	||
		object.LeftPoint()	> (T)mRect.RightPoint() ||
		object.BotPoint()	< (T)mRect.TopPoint()	||
		object.TopPoint()	> (T)mRect.BotPoint())
	{
		return false;
	}
	else
		return true;
}


template <typename T>
bool Camera::inView(Vector2D<T> object) const
{
	if (object.x < (T)mRect.LeftPoint() ||
		object.x > (T)mRect.RightPoint() ||
		object.y < (T)mRect.TopPoint() ||
		object.y > (T)mRect.BotPoint()
		)
	{
		return false;
	}
	else
		return true;
}


template <typename T>
Rect<T> Camera::toCameraCoords(const Rect<T> worldCoords) const
{
	return Rect<T>(worldCoords.TopLeft() - mActiveRect->TopLeft(), worldCoords.Size());
}