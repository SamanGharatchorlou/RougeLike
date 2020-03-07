#pragma once

#include <vector>

template<class T>
class RingBuffer
{
public:
	RingBuffer() : head(0), tail(0), full(false) { };

	T& operator [] (int i) { return buffer[i]; }

	void push(T object) { buffer.push_back(object); }

	T& front() { return buffer[head]; }

	int size() const { return buffer.size(); }

	void advance() 
	{ 
		increment(head);
		full = (head == tail);

		ASSERT(Warning, !full, "ring buffer is full with size %d, consider increasing the size\n", buffer.size());
	}

	void retreat() 
	{ 
		printf("retreat\n");
		if (tail != head)
			increment(tail);
		else
			DebugPrint(Warning, "Ring buffer is empty, cannot retreat tail further\n");
	}


private:
	void increment(unsigned int &index)
	{
		if (++index >= buffer.size())
			index = 0;
	}


private:
	std::vector<T> buffer;

	unsigned int head;
	unsigned int tail;

	bool full;
};