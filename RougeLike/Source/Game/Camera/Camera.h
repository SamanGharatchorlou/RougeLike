#pragma once

#include "CameraShake.h"
#include "Utilities/Quad2D.h"

class Camera
{

public:
	static Camera* Get();

	void clear();

	void setScale(float scale);
	float scale() const { return mScale; }

	void setViewport(VectorF viewport) { mRect.SetSize(viewport); }
	void setPosition(VectorF position);

	void setMapBoundaries(RectF boundaries) { mBoundaries = boundaries; }

	// follow this object
	void follow(RectF* rect);

	void slowUpdate(float dt);
	void fastUpdate(float dt);

	template <typename T>
	bool inView(const Rect<T>& object) const;

	template <typename T>
	bool inView(const Vector2D<T>& point) const;

	template <typename T>
	Rect<T> toCameraCoords(const Rect<T>& worldCoords) const;
	VectorF toCameraCoords(const VectorF& worldCoords) const;
	Quad2D<float> toCameraCoords(const Quad2D<float>& worldCoords) const;

	RectF rect() const { return mRect; }
	VectorF size() const { return mRect.Size(); }

	void initShakeyCam(float maxTrauma, float traumaReduction) { shakeyCam.init(maxTrauma, traumaReduction); }
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

	float mScale;
};


template <typename T>
bool Camera::inView(const Rect<T>& object) const
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
bool Camera::inView(const Vector2D<T>& object) const
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
Rect<T> Camera::toCameraCoords(const Rect<T>& worldCoords) const
{
	return Rect<T>(worldCoords.TopLeft() - mActiveRect->TopLeft(), worldCoords.Size());
}