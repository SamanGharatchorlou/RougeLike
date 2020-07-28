#pragma once


// Object are pulled from here then returned when finished with
template<class K, typename T>
class ObjectPool
{
public:
	ObjectPool() { }

	virtual void load() = 0;
	void freeAll();

	void addNewObjects(T type, int count);

	K* getObject(T type);
	virtual void returnObject(K* object) = 0;

private:
	virtual K* createNewObject(T type) const = 0;

protected:
	std::unordered_map<T, std::queue<K*>> mPool;
};


template<class K, typename T>
void ObjectPool<K, T>::addNewObjects(T type, int count)
{
	// TODO: what if mPool it already contains a queue of type T
	std::queue<K*> queue;

	for (int i = 0; i < count; i++)
	{
		K* object = createNewObject(type);
		queue.push(object);
	}

	mPool[type] = queue;
}



template<class K, typename T>
K* ObjectPool<K, T>::getObject(T type)
{
	if (mPool.count(type) > 0)
	{
		if (mPool[type].size() > 1)
		{
			K* object = mPool[type].front();
			mPool[type].pop();
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
		}
	}
}