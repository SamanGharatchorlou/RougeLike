#pragma once

#include "Trauma.h" // Do I need this?

#include "Events/Observer.h"


class CameraShake : public Observer
{
public:
	CameraShake(float maxTrauma, float traumaReduction, float maxAngle, VectorF maxOffset);

	void update(float dt);

	void handleEvent(const Event event, EventData& data) override;


	float angle();
	VectorF offset();

private:
	//Trauma trauma;
	int mTrauma;
	int mMaxTrauma;

	int mTraumaReduction;

	//float mAngle;
	float mMaxAngle;

	//VectorF mOffset;
	VectorF mMaxOffset;
};