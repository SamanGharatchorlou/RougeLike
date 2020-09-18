#pragma once

enum class TimeState
{
	Stopped,
	Running,
	Paused
};

template <typename T>
class Timer
{
public:
	enum Command
	{
		Stop,
		Start
	};

public:
	Timer(Command startingState = Stop);

	// Controls
	void start();
	void pause();
	void resume();
	void stop();
	void restart();

	T getMilliseconds() const;
	T getSeconds() const { return getMilliseconds() / 1000; }

	bool isPaused() const { return state == TimeState::Paused; }
	bool isStarted() const { return state == TimeState::Running || state == TimeState::Paused; }

private:
	T startTicks;
	T pauseTicks;

	TimeState state;
};

using TimerF = Timer<float>;


template <typename T>
Timer<T>::Timer(Command startingState) : startTicks(0), pauseTicks(0)
{
	if (startingState == Start)
	{
		state = TimeState::Running;
		start();
	}
	else
	{
		state = TimeState::Stopped;
	}
}


template <typename T>
inline void Timer<T>::start() 
{
	state = TimeState::Running;

	startTicks = static_cast<T>(SDL_GetTicks());
}


template <typename T>
inline void Timer<T>::pause()
{
	if(state == TimeState::Running)
	{
		state = TimeState::Paused;

		pauseTicks = SDL_GetTicks() - startTicks;
		startTicks = 0;
	}
}


template <typename T>
inline void Timer<T>::resume()
{
	if(state == TimeState::Paused)
	{
		state = TimeState::Running;

		startTicks = SDL_GetTicks() - pauseTicks;
		pauseTicks = 0;
	}
}


template <typename T>
inline void Timer<T>::stop()
{
	if(state == TimeState::Running || state == TimeState::Paused)
	{
		state = TimeState::Stopped;

		startTicks = 0; 
		pauseTicks = 0;
	}
}


template <typename T>
inline void Timer<T>::restart()
{ 
	state = TimeState::Running;

	pauseTicks = 0;
	startTicks = static_cast<T>(SDL_GetTicks());
}


template <typename T>
inline T Timer<T>::getMilliseconds() const
{
	if (state == TimeState::Running)
		return SDL_GetTicks() - startTicks;

	else if (state == TimeState::Paused)
		return pauseTicks;

	else
		return 0;
}