#include "pch.h"
#include "CameraShake.h"

CameraShake::CameraShake() :
	mTrauma(0), 
	mMaxTrauma(0), 
	mTraumaReduction(0)
{

}


void CameraShake::clear()
{
	mTrauma = 0;
	mMaxTrauma = 0;
	mTraumaReduction = 0;
}

void CameraShake::init(float maxTrauma, float traumaReduction)
{
	mMaxTrauma = maxTrauma;
	mTraumaReduction = traumaReduction;
}

void CameraShake::enable(RectF cameraRect, RectF boundaries)
{
	mCameraRect = cameraRect;
	mBoundaries = boundaries;
}


void CameraShake::fastUpdate(float dt)
{
	VectorF translation = offset();

#if !CAMERA_IGNORE_BOUNDARIES
	if (mCameraRect.LeftPoint() + translation.x >= mBoundaries.x1 &&
		mCameraRect.RightPoint() + translation.x <= mBoundaries.x2)
#endif
	{
		mCameraRect = mCameraRect.Translate(translation.x, 0.0f);
	}


#if !CAMERA_IGNORE_BOUNDARIES
	if (mCameraRect.TopPoint() + translation.y >= mBoundaries.y1 &&
		mCameraRect.BotPoint() + translation.y <= mBoundaries.y2)
#endif
	{
		mCameraRect = mCameraRect.Translate(0.0f, translation.y);
	}

	// dampen trauma;
	mTrauma -= mTraumaReduction * dt;
	mTrauma = std::max(0.0f, mTrauma);
}


void CameraShake::handleEvent(EventData& data)
{
	if (data.eventType == Event::Trauma)
	{
		TraumaEvent eventData = static_cast<TraumaEvent&>(data);

		mTrauma += eventData.mTrauma * 0.75f ;
		mTrauma = clamp(mTrauma, 0.0f, mMaxTrauma);
	}
}

void CameraShake::addTrauma(float trauma)
{
	mTrauma = clamp(mTrauma + trauma, 0.0f, mMaxTrauma);
}


VectorF CameraShake::offset()
{
	float xRandom = (float)randomNumberBetween(-100, 101) / 100;
	float yRandom = (float)randomNumberBetween(-100, 101) / 100;

	return (VectorF(xRandom, yRandom) / 2000) * (mTrauma * mTrauma);
}
