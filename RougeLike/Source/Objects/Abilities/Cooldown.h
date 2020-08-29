#pragma once

#include "Utilities/Timer.h"

class Cooldown
{
public:
	Cooldown() : mCooldownTime(0.0f) { }
	Cooldown(float time) : mCooldownTime(time) { }

	void set(float time) { mCooldownTime = time; }

	void stop() { mTimer.stop(); }

	void begin() { mTimer.restart(); }
	bool hasCompleted() const { return mTimer.getSeconds() > mCooldownTime; }
	bool hasStarted() const { return mTimer.isStarted(); }

	float timeElapsed() const { return mTimer.getSeconds(); }

private:
	float mCooldownTime;
	Timer<float> mTimer;
};