#include "pch.h"
#include "CameraShake.h"

CameraShake::CameraShake() :
	mTrauma(0), 
	mMaxTrauma(0), 
	mTraumaReduction(0), 
	mMaxAngle(0.0f)
{

}


void CameraShake::init(float maxTrauma, float traumaReduction, float maxAngle)
{
	mMaxTrauma = maxTrauma;
	mTraumaReduction = traumaReduction;
	mMaxAngle = maxAngle;
}


void CameraShake::update(float dt)
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


float CameraShake::angle()
{
	float randomNumber = (float)randomNumberBetween(-100, 100) / 100;
	return mTrauma * mMaxAngle * randomNumber;
}


VectorF CameraShake::offset()
{
	// TODO: check these two random number generations give actual random numbers
	float xRandom = (float)randomNumberBetween(-100, 100) / 100;
	float yRandom = (float)randomNumberBetween(-100, 100) / 100;

	printf("trauma %f\n", mTrauma);
	return (VectorF(xRandom, yRandom) / 2000) * (mTrauma * mTrauma);
}
