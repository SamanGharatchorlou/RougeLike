#pragma once


class PerformanceProfiler
{
public:
	PerformanceProfiler(BasicString name);

	void start() { mTimer.start(); }
	void restart() { mTimer.restart(); }

	void displayTimeSeconds();
	void displayTimeMilliSeconds();

	void displayAverageTimeSeconds();
	void displayAverageTimeMilliSeconds();
	void displayAverageTimeEvery(float seconds);

	void clearAverageTimeEvery(float seconds) { averageResetTime = seconds; }
	void saveToAverage();


private:
	const BasicString mName;
	TimerF mTimer;

	float mTotalTime;
	int mCount;

	TimerF mDisplayTimer;

	float averageResetTime;
	TimerF averageResetTimer;
};