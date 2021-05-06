#pragma once

#include <deque>


template <class T>
class Queue
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

	void clear() { mQueue.clear(); }

	T& pop();
	virtual void push(T value) { mQueue.push_back(value); }

	void concat(const Queue<T>& otherQueue);

	bool contains(const T& value) const;
	int size() const { return mQueue.size(); }

protected:
	std::deque<T> mQueue;
};


template <class T>
void Queue<T>::concat(const Queue<T>& otherQueue)
{
	mQueue.insert(mQueue.end(), otherQueue.begin(), otherQueue.end());
}

// TODO: Warning this function is broken with the BasicString class because the string deletes itself when it goes out of scope (use get then pop)
// Really this applies to any class: if you have a value type i.e. not a pointer in the queue then pop() to get a reference it goes out of scope
// so the reference you have is junk. Use std::move to get aound this.
template <class T>
T& Queue<T>::pop()
{
	T& value = front();
	mQueue.pop_front();
	return value;
}

template <class T>
bool Queue<T>::contains(const T& value) const
{
	for (Queue::const_iterator iter = mQueue.begin(); iter != mQueue.end(); iter++)
	{
		if (*iter == value)
			return true;
	}

	return false;
}


template <class T>
T& Queue<T>::get(int index)
{
	ASSERT(Error, index < mQueue.size(), "Out of bounds access for Queue container\n");

	Queue::iterator iter = mQueue.begin();
	return *(iter + index);
}