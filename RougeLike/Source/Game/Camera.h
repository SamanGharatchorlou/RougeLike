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
	bool inView(Rect<T> object);

	VectorF getCenter() { return mRect.Center(); }
	Vector2D<int> getCenterI() { return Vector2D<int>(mRect.Center()); }
	
	VectorF getTopLeftF() { return mRect.TopLeft(); }
	Vector2D<int> getTopLeftI() { return Vector2D<int>(mRect.TopLeft()); }

	// TODO: make some of this stuff const
	VectorF toCameraCoords(VectorF worldCoords);
	RectF toCameraCoords(RectF worldRect);
	Rect<int> toCameraCoords(Rect<int> worldRect);

	VectorF size() const { return mRect.Size(); }

	void initShakeyCam(float maxTrauma, float traumaReduction, float maxAngle) { shakeyCam.init(maxTrauma, traumaReduction, maxAngle); }
	CameraShake* getShake() { return &shakeyCam; }


private:
	Camera();
	~Camera() { }


private:
	RectF mRect;
	RectF mBoundaries;
	const RectF* mActiveRect;
	RectF* mFollowingRect;

	CameraShake shakeyCam;
};


template <typename T>
bool Camera::inView(Rect<T> object)
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