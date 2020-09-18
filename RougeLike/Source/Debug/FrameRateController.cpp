#include "pch.h"
#include "FrameRateController.h"

FrameRateController::FrameRateController() : dt(0.0f)
{
#if PRINT_FRAMERATE_EVERY
	float fpsSum = 0.0f;
	int fpsCounter = 0;
	float totalFrameTime = 0.0f;
	int totalFrames = 0;
	fpsTimer.start();
#endif
}

void FrameRateController::start()
{
	frameTimer.start();

#if FRAMERATE_CAP
	frameRateCap = FRAMERATE_CAP;
	capTimer.start();
#endif
#if PRINT_FRAMERATE_EVERY
	fpsTimer.start();
#endif
}


void FrameRateController::update()
{
	dt = frameTimer.getSeconds();

#if PRINT_FRAMERATE_EVERY
	printfFrameRate();
#endif

	frameTimer.restart();

#if FRAMERATE_CAP
	capFrameRate();
#endif
}


void FrameRateController::resetCapTimer()
{
	capTimer.restart();
}

void FrameRateController::capFrameRate()
{
	//If frame finished early
	int frameTicks = (int)capTimer.getMilliseconds();
	if (frameTicks < (1000 / frameRateCap))
	{
		//Wait remaining time
		SDL_Delay((1000 / (int)frameRateCap) - frameTicks);
	}
}


#if PRINT_FRAMERATE_EVERY
void FrameRateController::printfFrameRate()
{
	fpsSum += frameTimer.getMilliseconds();
	fpsCounter++;

	totalFrameTime += frameTimer.getMilliseconds();
	totalFrames++;

	if (fpsTimer.getMilliseconds() > PRINT_FRAMERATE_EVERY)
	{
		fpsSum /= fpsCounter;

		printf("framerate %f | Running fps average %f\n", 1000 / fpsSum, 1000 / (totalFrameTime / totalFrames));
		fpsSum = 0.0f;
		fpsCounter = 0;
		fpsTimer.restart();
	}
}
#endif