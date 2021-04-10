#include "pch.h"
#include "PerformanceProfiler.h"

PerformanceProfiler::PerformanceProfiler(BasicString name) : mName(name), mTotalTime(0.0f), mCount(0), averageResetTime(-1.0f)
{
	mDisplayTimer.start();
	averageResetTimer.start();
}

void PerformanceProfiler::displayAverageTimeEvery(float seconds)
{
	if (mDisplayTimer.getSeconds() > seconds)
	{
		DebugPrint(Profile, "Profiler %s average time %fms\n", mName.c_str(), mTotalTime / mCount);
		mDisplayTimer.restart();
	}

	if (averageResetTime > 0.0f && averageResetTimer.getSeconds() > averageResetTime)
	{
		mTotalTime = 0.0f;
		mCount = 0;
		averageResetTimer.restart();
	}
}


void PerformanceProfiler::saveToAverage()
{
	mTotalTime += mTimer.getMilliseconds();
	mCount++;
}


void PerformanceProfiler::displayTimeSeconds()
{
	DebugPrint(Profile, "Profiler %s time %fs\n", mName.c_str(), mTimer.getSeconds());
}


void PerformanceProfiler::displayTimeMilliSeconds()
{
	DebugPrint(Profile, "Profiler %s time %fms\n", mName.c_str(), mTimer.getMilliseconds());
}


void PerformanceProfiler::displayAverageTimeSeconds()
{
	DebugPrint(Profile, "Profiler %s average time %fs\n", mName.c_str(), mTotalTime * 0.001f / mCount);
}


void PerformanceProfiler::displayAverageTimeMilliSeconds()
{
	DebugPrint(Profile, "Profiler %s average time %fms\n", mName.c_str(), mTotalTime / mCount);
}