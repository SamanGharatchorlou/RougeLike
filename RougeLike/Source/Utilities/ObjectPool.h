#pragma once


// Object are pulled/removed from the pool then must be returned when finished with
template<class K, typename T>
class ObjectPool
{
public:
	ObjectPool() { }
	virtual ~ObjectPool() { freeAll(); }

	void load(const std::vector<T>& types, int count);
	void freeAll();

	void addNewObjects(T type, int count);

	K* getObject(T type);
	void returnObject(K* object, T type);

	int size(T type) { return mPool[type].size(); }

private:
	virtual K* createNewObjects(T type, int count, int& objectSize) const = 0;

protected:
	std::unordered_map<T, std::queue<K*>> mPool;

	// Keep a list of all the allocated blocks, for deleting.
	std::vector<K*> mBlockHeads;
};


template<class K, typename T>
void ObjectPool<K, T>::load(const std::vector<T>& types, int count)
{
	for (T type : types)
	{
		addNewObjects(type, count);
	}
}


template<class K, typename T>
void ObjectPool<K, T>::addNewObjects(T type, int count)
{
	// Move all the objects into a queue ready for use
	std::queue<K*>& queue = mPool[type];

	// we new[] blocks of derived classes and only have the base class pointers so we need the derived class size (objectSize)
	int objectSize = 0;
	K* objects = createNewObjects(type, count, objectSize);
	mBlockHeads.push_back(objects);

	if (objectSize > 0)
	{
		for (int i = 0; i < count; i++)
		{
			// Shift the pointer across a 'objectSize' number of bytes so we're pointing at the correct block
			char* pointer = (char*)objects + i * objectSize;
			queue.push((K*)pointer);
		}
	}
}



template<class K, typename T>
K* ObjectPool<K, T>::getObject(T type)
{
	if (mPool.count(type) > 0)
	{
		if (mPool.at(type).size() > 0)
		{
			K* object = mPool[type].front();
			mPool[type].pop();
			return object;
		}
		else
		{
			addNewObjects(type, 10);
			DebugPrint(Warning, "Not enough objects in the pool, size increased by 10\n");
			return getObject(type);
		}
	}

	DebugPrint(Warning, "No objects of requested type have been setup in the pool, use addNewObjects() before\n");
	return nullptr;
}


template<class K, typename T>
void ObjectPool<K, T>::returnObject(K* object, T type)
{
	mPool[type].push(object);
}


template<class K, typename T>
void ObjectPool<K, T>::freeAll()
{
	for (int i = 0; i < mBlockHeads.size(); i++)
	{
		K* block = mBlockHeads[i];
		delete[] block;
	}

	mBlockHeads.clear();
	mPool.clear();
}