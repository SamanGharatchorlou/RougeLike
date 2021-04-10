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
			if (count == 0)
			{
				DebugPrint(Profiling, "Profiler %s: no average time saved\n", name.c_str());
			}
			else
			{
				DebugPrint(Profiling, "Profiler %s average time %fms (%d)\n", name.c_str(), totalTime / count, count);
			}
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