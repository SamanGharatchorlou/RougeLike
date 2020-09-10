#pragma once

#include "Queue.h"

template <class T>
class UniqueQueue : public Queue<T>
{
public:
	// QUESTION: why can i only access mQueue by Queue<T>::mQueue?
	void push(T value) override
	{
		for (typename std::deque<T>::iterator iter = Queue<T>::mQueue.begin(); iter != Queue<T>::mQueue.end(); iter++)
		{
			if (*iter == value)
				return;
		}

		Queue<T>::mQueue.push_back(value);
	}
};
