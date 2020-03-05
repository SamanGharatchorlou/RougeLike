#include "pch.h"
#include "CameraShake.h"

CameraShake::CameraShake(float maxTrauma, float traumaReduction, float maxAngle, VectorF maxOffset) :
	mTrauma(0), 
	mMaxTrauma(maxTrauma), 
	mTraumaReduction(traumaReduction), 
	mMaxAngle(maxAngle), 
	mMaxOffset(maxOffset)
{

}


void CameraShake::update(float dt)
{
	// dampen trauma;
	mTrauma -= mTraumaReduction * dt;
	mTrauma = std::min(0, mTrauma);
}


void CameraShake::handleEvent(const Event event, EventData& data)
{
	if (event == Event::Trauma)
	{
		TraumaEvent eventData = static_cast<TraumaEvent&>(data);

		mTrauma += eventData.mTrauma;

		mTrauma = clamp(mTrauma, 0, mMaxTrauma);
		printf("trauma increased %d\n", mTrauma);
	}
}

// use perlin noise

float CameraShake::angle()
{
	float randomNumber = randomNumberBetween(-1.0f, +1.0f);
	return mTrauma * mMaxAngle * randomNumber;
}


VectorF CameraShake::offset()
{
	// TODO: check these two random number generations give actual random numbers
	float xRandom = randomNumberBetween(-1.0f, +1.0f);
	float yRandom = randomNumberBetween(-1.0f, +1.0f);

	return mMaxOffset * VectorF(xRandom, yRandom) * mTrauma;
}