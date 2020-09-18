#pragma once


class PerformanceProfiler
{
public:
	PerformanceProfiler() : time(0.0f), totalTime(0.0f), count(0), averageResetTime(-1.0f)
	{ 
		displayTimer.start();
		averageResetTimer.start();
	}

	void start()
	{
		timer.restart();
	}

	void setTime()
	{
		time = timer.getMilliseconds();
	}
	void addTime()
	{
		time += timer.getMilliseconds();
	}

	bool displayAverageTimeEvery(float seconds)
	{
		bool didPrint = false;

		if (displayTimer.getSeconds() > seconds)
		{
			printf("Profiler %s average time %fms\n", name.c_str(), totalTime / count);
			displayTimer.restart();
			didPrint = true;
		}

		if (averageResetTime > 0.0f && averageResetTimer.getSeconds() > averageResetTime)
		{
			totalTime = 0.0f;
			count = 0;
			averageResetTimer.restart();
		}

		return didPrint;
	}

	void saveToAverage()
	{
		totalTime += time;
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