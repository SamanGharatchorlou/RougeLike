#pragma once


// Object are pulled from here then returned when finished with
template<class K, typename T>
class ObjectPool
{
public:
	ObjectPool() { }
	virtual ~ObjectPool() { freeAll(); }

	void load(std::vector<T>& types, int count);
	void freeAll();

	void addNewObjects(T type, int count);

	K* getObject(T type);
	void returnObject(K* object, T type);

	int size(T type) { return mPool[type].size(); }

private:
	virtual K* createNewObject(T type) const = 0;

protected:
	std::unordered_map<T, std::queue<K*>> mPool;

#if DEBUG_CHECK
	int objectCount = 0;
#endif
};


template<class K, typename T>
void ObjectPool<K, T>::load(std::vector<T>& types, int count)
{
	for (T type : types)
	{
		addNewObjects(type, count);
	}
}


template<class K, typename T>
void ObjectPool<K, T>::addNewObjects(T type, int count)
{
	std::queue<K*> queue = mPool[type];

	for (int i = 0; i < count; i++)
	{
		K* object = createNewObject(type);
		queue.push(object);
#if DEBUG_CHECK
		objectCount++;
#endif
	}

	mPool[type] = queue;
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
	typename std::unordered_map<T, std::queue<K*>>::iterator iter;

	for (iter = mPool.begin(); iter != mPool.end(); iter++)
	{
		std::queue<K*> queue = iter->second;
		while (!queue.empty())
		{
			K* object = queue.front();
			delete object;
			queue.pop();
#if DEBUG_CHECK
			objectCount--;
#endif
		}
	}

	mPool.clear();

#if DEBUG_CHECK
	if (objectCount != 0)
	{
		DebugPrint(Warning, "Object Pool object count after free all != 0. There's probably %d objects left floating around undeleted!\n", objectCount);
	}
#endif
}