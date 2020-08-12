#pragma once

class XMLNode;

template <class T, class K>
class DataMap
{
public:
	// Iterator
	using iterator = typename std::unordered_map<T, K>::iterator;
	iterator begin() { return mData.begin(); }
	iterator end() { return mData.end(); }

	// Const Iterator	
	using const_iterator = typename std::unordered_map<T, K>::const_iterator;
	const_iterator begin() const { return mData.begin(); }
	const_iterator end() const { return mData.end(); }


public:
	virtual void fill(const XMLNode& node) = 0;
	virtual ~DataMap() { };

	void empty() { mData.clear(); }

	K& operator [] (const T& key) { return mData[key]; }
	const K& at(const T& key) const;

	int size() const { mData.size(); }
	bool contains(const T& key) const { return mData.count(key) > 0; }
	bool isEmpty() const { return mData.size() == 0; }

	void merge(const DataMap<T, K>& map);

#if _DEBUG
	void log() const;
#endif

protected:
	std::unordered_map<T, K> mData;
};

#if _DEBUG
template<class T, class K>
void DataMap<T,K>::log() const
{
	DebugPrint(Log, "\n-String Data Map Values-\n");
	const_iterator iter;
	for (iter = mData.begin(); iter != mData.end(); iter++)
	{
		BasicString key(iter->first);
		BasicString value(iter->second);
		DebugPrint(Log, "Data map %s has value %s\n", key.c_str(), value.c_str());
	}
	DebugPrint(Log, "----------------\n");
}
#endif


template <class T, class K>
const K& DataMap<T, K>::at(const T& key) const
{
	return mData.at(key);
}


template <class T, class K>
void DataMap<T, K>::merge(const DataMap<T, K>& map)
{
	mData.insert(map.begin(), map.end());
}

