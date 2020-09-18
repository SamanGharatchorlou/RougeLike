#pragma once

#include "Debug/DebugDraw.h"

class FrameRateController
{
public:
	FrameRateController();

	float delta() const { return dt; }
	void start();
	void resetAverage() { /*totalFrameTime = 0.0f; totalFrames = 0;*/ }
	void update();

	void setFrameCap(float cap) { frameRateCap = cap; }
	void resetCapTimer();
	void capFrameRate();

#if PRINT_FRAMERATE_EVERY
	void printfFrameRate();
#endif


private:
	TimerF frameTimer;
	float dt;

	TimerF capTimer;
	float frameRateCap;

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