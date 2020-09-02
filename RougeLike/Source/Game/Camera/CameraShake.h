#pragma once

#include "Events/Observer.h"


class CameraShake : public Observer
{
public:
	CameraShake();

	void init(float maxTrauma, float traumaReduction);
	void fastUpdate(float dt);

	void setRect(RectF rect) { cameraRect = rect; }

	void handleEvent(EventData& data) override;

	VectorF offset();

	RectF* rect() { return &cameraRect; }

	float trauma() const { return mTrauma; }
	bool hasTrauma() const { return mTrauma != 0; }




private:
	float mTrauma;
	float mMaxTrauma;
	float mTraumaReduction;

	RectF cameraRect;
};