#pragma once

#include <vector>

template<class T>
class Buffer
{
public:
	T& operator [] (int i) { return buffer[i]; }

	void add(T object) { buffer.push_back(object); }

	T getAvailable();
	void insert(T object);

	int size() const { return buffer.size(); }

private:
	std::vector<T> buffer;
};

template<class T>
T Buffer<T>::getAvailable()
{
	for (unsigned int i = 0; i < buffer.size(); i++)
	{
		if (buffer[i] != nullptr)
			return buffer[i];
	}

	ASSERT(Warning, false, "Buffer has ran out of size, %d used objects\n", buffer.size());
	// TODO: FIX cannot return nullptr because it might not only be pointerss
	// return NULL?
	return NULL;
}

template<class T>
void Buffer<T>::insert(T object)
{
	for (unsigned int i = 0; i < buffer.size(); i++)
	{
		if (buffer[i] = nullptr)
			buffer[i] = object;
	}
}