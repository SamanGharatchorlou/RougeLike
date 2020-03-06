#pragma once

#include <vector>

template<class T>
class Queue
{
public:

	T& operator [] (int i) { return queue[i]; }

	T& front() { return queue[0]; }

	T& pop_front();
	void push_back(T object) { queue.push_back(object); }

	int size() const { return queue.size(); }


private:
	std::vector<T> queue;
};



template<class T>
T& Queue<T>::pop_front()
{
	T& frontObject = queue[0];

	for (unsigned int i = 0; i < queue.size() - 1; i++)
	{
		queue[i] = queue[i + 1];
		queue.pop_back();
	}

	return frontObject;
}