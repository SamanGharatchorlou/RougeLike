#pragma once

#include "Debug/DebugDraw.h"

class FrameRateController
{
public:
	FrameRateController();

	float delta() const { return dt; }
	void start();
	void update();

#if FRAMERATE_CAP
	void resetCapTimer();
	void capFrameRate();
#endif

#if PRINT_FRAMERATE_EVERY
	void printfFrameRate();
#endif


private:
	TimerF frameTimer;
	float dt;


	// --- debugging --- //
#if PRINT_FRAMERATE_EVERY
	float fpsSum = 0.0f;
	int fpsCounter = 0;
	float totalFrameTime = 0.0f;
	int totalFrames = 0;
#endif

#if FRAMERATE_CAP
	TimerF capTimer;
#endif
#if PRINT_FRAMERATE_EVERY
	TimerF fpsTimer;
#endif
};