#pragma once

#include <deque>


template <class T>
class UniqueQueue
{
public:
	T& front() { return mQueue.front(); }
	T& popFront();

	void clear() { mQueue.clear(); }

	void push(T value);

	bool contains(const T& value) const;
	int size() const { return mQueue.size(); }

private:
	std::deque<T> mQueue;
};


template <class T>
T& UniqueQueue<T>::popFront()
{
	T& value = front();
	mQueue.pop_front();
	return value;
}


template <class T>
void UniqueQueue<T>::push(T value)
{
	for (typename std::deque<T>::iterator iter = mQueue.begin(); iter != mQueue.end(); iter++)
	{
		if (*iter == value)
			return;
	}

	mQueue.push_back(value);
}


template <class T>
bool UniqueQueue<T>::contains(const T& value) const
{
	for (typename std::deque<T>::const_iterator iter = mQueue.begin(); iter != mQueue.end(); iter++)
	{
		if (*iter == value)
			return true;
	}

	return false;
}
