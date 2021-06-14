#pragma once

// SGT: TODO remove all theDebugDraw include, its in the pch now
#include "Debug/DebugDraw.h"

class FrameRateController
{
public:
	FrameRateController();

	float delta() const { return dt; }
	void start();
	void update();

	void setFrameCap(float cap) { frameRateCap = cap; }
	void resetCapTimer();
	void capFrameRate();

	unsigned long frameCount() const { return frameNumber; }

#if PRINT_FRAMERATE_EVERY
	void printfFrameRate();
#endif


private:
	TimerF frameTimer;
	float dt;

	TimerF capTimer;
	float frameRateCap;

	unsigned long frameNumber;

	// --- debugging --- //
#if PRINT_FRAMERATE_EVERY
	float fpsSum = 0.0f;
	int fpsCounter = 0;
	float totalFrameTime = 0.0f;
	int totalFrames = 0;
#endif


#if PRINT_FRAMERATE_EVERY
	TimerF fpsTimer;
#endif
};