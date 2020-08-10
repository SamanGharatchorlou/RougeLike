#pragma once

#include "BasicString.h"
#include <unordered_map>


template<class T>
class DataMap
{
public:
	T& operator [] (const BasicString& label) { return data[label]; }
	T& at(const BasicString& label) { return data.at(label); }
	const T& at(const BasicString& label) const { return data.at(label); }

	// Iterator
	using iterator = typename std::unordered_map<BasicString, T>::iterator;
	iterator begin() { return data.begin(); }
	iterator end() { return data.end(); }

	// Const Iterator	
	using const_iterator = typename std::unordered_map<BasicString, T>::const_iterator;
	const_iterator begin() const { return data.begin(); }
	const_iterator end() const { return data.end(); }


public:
	std::unordered_map<BasicString, T>& getMap() { return data; }
	const std::unordered_map<BasicString, T>& getMap() const { return data; }

	void set(DataMap<T> map);
	void merge(const DataMap<T>& map);

	void add(const BasicString& name, T value);

	bool contains(const BasicString& name) const;
	bool empty() const { return data.empty(); }


	BasicString getString(const BasicString& name) const;
	int getInt(const BasicString& name) const;
	int getBool(const BasicString& name) const;
	float getFloat(const BasicString& name) const;

#if _DEBUG
	void log() const;
	void checkValue(const BasicString& label) const;
#endif


private:
	std::unordered_map<BasicString, T> data;
};


template<typename T>
void DataMap<T>::set(DataMap<T> map)
{
	data = map.getMap();
}


template<typename T>
void DataMap<T>::merge(const DataMap<T>& map)
{
	data.insert(map.getMap().begin(), map.getMap().end());
}


template<typename T>
void DataMap<T>::add(const BasicString& label, T value)
{
#if _DEBUG
	if (contains(label))
		DebugPrint(Warning, "Adding label '%s' with value '%s' where a value already exists, overriting previous value\n", label.c_str(), BasicString(value));
#endif
	data[label] = value;
}


template<typename T>
bool DataMap<T>::contains(const BasicString& label) const
{
	return (bool)data.count(label);
}


template<typename T>
BasicString DataMap<T>::getString(const BasicString& label) const
{
#if _DEBUG
	checkValue(label);
#endif
	return BasicString(data.at(label));
}


template<typename T>
int DataMap<T>::getInt(const BasicString& label) const
{
#if _DEBUG
	checkValue(label);
#endif
	return atoi(data.at(label).c_str());
}


template<typename T>
int DataMap<T>::getBool(const BasicString& label) const
{
#if _DEBUG
	checkValue(label);
#endif
	return data.at(label) == "true";
}


template<typename T>
float DataMap<T>::getFloat(const BasicString& label) const
{
#if _DEBUG
	checkValue(label);
#endif
	return atof(data.at(label).c_str());
}


#if _DEBUG
template<typename T>
void DataMap<T>::log() const
{
	DebugPrint(Log, "\n-String Data Map Values-\n");
	typename std::unordered_map<BasicString, T>::const_iterator iter;
	for (iter = data.begin(); iter != data.end(); iter++)
	{
		DebugPrint(Log, "Data map %s has value %s\n", iter->first.c_str(), getString(iter->first).c_str());
	}
	DebugPrint(Log, "----------------\n");
}

template<typename T>
void DataMap<T>::checkValue(const BasicString& label) const
{
	if (!contains(label))
		DebugPrint(Warning, "There is no value with the label: %s\n", label.c_str());
}
#endif

