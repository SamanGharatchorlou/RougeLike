#pragma once

#include <deque>


template <class T>
class UniqueQueue
{
public:
	// Iterator
	using iterator = typename std::deque<T>::iterator;
	iterator begin() { return mQueue.begin(); }
	iterator end() { return mQueue.end(); }

	// Const Iterator	
	using const_iterator = typename std::deque<T>::const_iterator;
	const_iterator begin() const { return mQueue.begin(); }
	const_iterator end() const { return mQueue.end(); }

	// reverse
	using const_rIterator = typename std::deque<T>::const_reverse_iterator;
	const_rIterator rbegin() const { return mQueue.rbegin(); }
	const_rIterator rend() const { return mQueue.rend(); }


public:
	T& front() { return mQueue.front(); }
	const T& front() const { return mQueue.front(); }

	T& back() { return mQueue.back(); }
	const T& back() const { return mQueue.back(); }

	T& get(int i);

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


template <class T>
T& UniqueQueue<T>::get(int index)
{
	for (typename std::deque<T>::iterator iter = mQueue.begin(); iter != mQueue.end(); iter++)
	{
		if (index == 0)
			return *iter;

		index--;
	}
}