#pragma once

#include "Utilities/Vector2D.h"

using Index = Vector2D<int>;

template<class K>
class MapBase
{
public:
	MapBase() { }
	MapBase(Vector2D<int> size, K value) : mData(size, value) { }

	K& operator [] (Index index) { 
		return mData[index.y][index.x]; 
	}
	const Grid<K>& getData() const { return mData; }
	Grid<K>& getData() { return mData; }

	unsigned int yCount() const { return mData.yCount(); }
	unsigned int xCount() const { return mData.xCount(); }

	const K* tile(Index index) const;

	bool inBounds(Index index) const;
	bool isValidIndex(Index index) const;

protected:
	Grid<K> mData;
};


template<class K>
const K* MapBase<K>::tile(const Index index) const
{
	return isValidIndex(index) ? &mData.get(index) : nullptr;
}


template <class K>
bool MapBase<K>::isValidIndex(Index index) const
{
	return (index.x >= 0 && index.x < xCount()) &&
		(index.y >= 0 && index.y < yCount());
}


template <class K>
bool MapBase<K>::inBounds(Index index) const
{
	bool xBounds = (index.x >= 0 && index.x < xCount());
	bool yBounds = (index.y >= 0 && index.y < yCount());

	return xBounds && yBounds;
}
