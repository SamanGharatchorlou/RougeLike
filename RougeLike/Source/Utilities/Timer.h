#pragma once

template <typename T>
class Timer
{
public:
	enum Command
	{
		Stop,
		Start
	};

private:
	enum State
	{
		kStopped,
		kRunning,
		kPaused
	};

public:
	Timer(Command startingState = Stop);

	// Timer Controls
	void start();
	void pause();
	void resume();
	void stop();
	void restart();

	// Get time passed
	T getMilliseconds() const;
	T getSeconds() const { return getMilliseconds() / 1000; }

	// Timer State
	bool isPaused() const { return state == kPaused; }
	bool isStarted() const { return state == kRunning || state == kPaused; }

private:
	T startTicks;
	T pauseTicks;

	State state;
};

using TimerF = Timer<float>;


template <typename T>
Timer<T>::Timer(Command startingState) : startTicks(0), pauseTicks(0)
{
	if (startingState == Start)
	{
		state = kRunning;
		start();
	}
	else
	{
		state = kStopped;
	}
}


template <typename T>
inline void Timer<T>::start() 
{
	state = kRunning;

	startTicks = static_cast<T>(SDL_GetTicks());
}


template <typename T>
inline void Timer<T>::pause()
{
	if(state == kRunning)
	{
		state = kPaused;

		pauseTicks = SDL_GetTicks() - startTicks;
		startTicks = 0;
	}
}


template <typename T>
inline void Timer<T>::resume()
{
	if(state == kPaused)
	{
		state = kRunning;

		startTicks = SDL_GetTicks() - pauseTicks;
		pauseTicks = 0;
	}
}


template <typename T>
inline void Timer<T>::stop()
{
	if(state == kRunning || state == kPaused)
	{
		state = kStopped;

		startTicks = 0; 
		pauseTicks = 0;
	}
}


template <typename T>
inline void Timer<T>::restart()
{ 
	state = kRunning;

	pauseTicks = 0;
	startTicks = static_cast<T>(SDL_GetTicks());
}


template <typename T>
inline T Timer<T>::getMilliseconds() const
{
	if (state == kRunning)
		return SDL_GetTicks() - startTicks;

	else if (state == kPaused)
		return pauseTicks;

	else
		return 0;
}