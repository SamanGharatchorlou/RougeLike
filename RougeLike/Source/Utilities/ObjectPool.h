#pragma once


template<class K, typename T>
class ObjectPool
{
public:
	ObjectPool() { };

	//void load();
	//void slowUpdate();

	void addNewObjects(K object, T type, int count);

	K getObject(T type);
	//void returnObject(K object);

private:
	virtual K createNewObject(T type) const = 0;

	//K getNewEffect(T type);


private:
	// Effects are pulled from this pool then returned when finished with
	std::unordered_map<T, std::queue<K*>> mPool;
	std::unordered_map<T, int> mPoolSizes;

#if _DEBUG // Tracker contains all effects at all times
	std::unordered_map<T, std::queue<K*>> mTrackerPool;
#endif
};


template<class K, typename T>
void ObjectPool<K, T>::addNewObjects(K object, T type, int count)
{
	// TODO: what if mPool it already contains a queue of type T
	std::queue<K> queue;

	for (int i = 0; i < count; i++)
	{
		queue.push(new K);
	}

	mPool[type] = queue;
}



template<class K, typename T>
K ObjectPool<K, T>::getObject(T type)
{
	if (mPool.count(type) > 0)
	{
		if (mPool[T].size() > 1)
		{
			K* object = mPool[T].front();
			mPool[T].pop();
			return object;
		}
		else
		{

		}
			// TODO: add new objects;
	}
	else
		DebugPrint(Warning, "No objects of requested type have been setup in the pool, use addNewObjects() before\n");
}