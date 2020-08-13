#pragma once

#include <vector>

template<class T>
class Queue
{
public:

	//T& operator [] (int i) { return queue[i]; }
	//T get(unsigned int i) const { return queue[i]; }

	T& front() { return queue.front(); }

	T& pop();
	void push(T object) { queue.push(object); }

	int size() const { return queue.size(); }


private:
	std::queue<T> queue;
};



template<class T>
T& Queue<T>::pop()
{
	T& frontObject = queue.front();
	queue.pop();
	return frontObject;
}