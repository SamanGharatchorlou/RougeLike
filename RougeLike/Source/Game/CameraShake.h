#pragma once

#include "Events/Observer.h"


class CameraShake : public Observer
{
public:
	CameraShake();

	void init(float maxTrauma, float traumaReduction, float maxAngle);
	void update(float dt);

	void setRect(RectF rect) { cameraRect = rect; }

	void handleEvent(const Event event, EventData& data) override;

	float angle();
	VectorF offset();

	RectF* rect() { return &cameraRect; }

	bool hasTrauma() const { return mTrauma != 0; }


private:
	float mTrauma;
	float mMaxTrauma;
	float mTraumaReduction;

	float mMaxAngle;

	RectF cameraRect;
};