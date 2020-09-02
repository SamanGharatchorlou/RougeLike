#include "pch.h"
#include "CameraShake.h"

CameraShake::CameraShake() :
	mTrauma(0), 
	mMaxTrauma(0), 
	mTraumaReduction(0)
{

}


void CameraShake::init(float maxTrauma, float traumaReduction)
{
	mMaxTrauma = maxTrauma;
	mTraumaReduction = traumaReduction;
}


void CameraShake::fastUpdate(float dt)
{
	cameraRect = cameraRect.Translate(offset());

	// dampen trauma;
	mTrauma -= mTraumaReduction * dt;
	mTrauma = std::max(0.0f, mTrauma);
}


void CameraShake::handleEvent(EventData& data)
{
	if (data.eventType == Event::Trauma)
	{
		TraumaEvent eventData = static_cast<TraumaEvent&>(data);

		mTrauma += eventData.mTrauma;

		mTrauma = clamp(mTrauma, 0.0f, mMaxTrauma);
	}
}


VectorF CameraShake::offset()
{
	float xRandom = (float)randomNumberBetween(-100, 101) / 100;
	float yRandom = (float)randomNumberBetween(-100, 101) / 100;

	return (VectorF(xRandom, yRandom) / 2000) * (mTrauma * mTrauma);
}
