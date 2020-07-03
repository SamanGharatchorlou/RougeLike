#pragma once

#include "Utilities/Timer.h"

class Cooldown
{
public:
	Cooldown() : mCooldownTime(0.0f) { }
	Cooldown(float time) : mCooldownTime(time) { }

	void stop() { mTimer.stop(); }

	void begin() { mTimer.restart(); }
	bool completed() const { return mTimer.getSeconds() > mCooldownTime; }

	float timeElapsed() const { return mTimer.getSeconds(); }

private:
	float mCooldownTime;
	Timer<float> mTimer;
};