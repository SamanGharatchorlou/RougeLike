#pragma once


class PerformanceProfiler
{
public:
	PerformanceProfiler() : time(0.0f), totalTime(0.0f), count(0), averageResetTime(-1.0f)
	{ 
		displayTimer.start();
		averageResetTimer.start();
	}

	void restart()
	{
		timer.restart();
	}

	void displayAverageTimeEvery(float seconds)
	{
		if (displayTimer.getSeconds() > seconds)
		{
			DebugPrint(Log, "Profiler %s average time %fms\n", name.c_str(), totalTime / count);
			displayTimer.restart();
		}

		if (averageResetTime > 0.0f && averageResetTimer.getSeconds() > averageResetTime)
		{
			totalTime = 0.0f;
			count = 0;
			averageResetTimer.restart();
		}
	}

	void saveToAverage()
	{
		totalTime += timer.getMilliseconds();
		count++;
	}

	BasicString name;
	TimerF timer;
	float time;

	float totalTime;
	int count;

	TimerF displayTimer;

	float averageResetTime;
	TimerF averageResetTimer;
};