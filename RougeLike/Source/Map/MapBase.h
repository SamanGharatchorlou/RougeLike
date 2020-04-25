#pragma once

#include "Utilities/Vector2D.h"

using Index = Vector2D<int>;

template<class K>
class MapBase
{
public:
	K& operator [] (Index index) { 
		return mData[index.y][index.x]; 
	}
	Grid<K>& getData() { return mData; }

	unsigned int yCount() const { return mData.yCount(); }
	unsigned int xCount() const { return mData.xCount(); }

	const K* tile(Index index) const;
	const K* tile(int x, int y) const;

	bool inBounds(int xIndex, int yIndex) const;

protected:
	bool isValidIndex(Index index) const;

protected:
	Grid<K> mData;
};


template<class K>
const K* MapBase<K>::tile(const Index index) const
{
	return isValidIndex(index) ? &mData.get(index) : nullptr;
}


template<class K>
const K* MapBase<K>::tile(int x, int y) const
{
	return isValidIndex(Vector2D<int>(x, y)) ? &mData.get(x, y) : nullptr;
}


template <class K>
bool MapBase<K>::isValidIndex(Index index) const
{
	bool a = (index.x >= 0 && index.x < xCount()) &&
		(index.y >= 0 && index.y < yCount());

	return a;
}


template <class K>
bool MapBase<K>::inBounds(int x, int y) const
{
	bool xBounds = (x >= 0 && x < xCount());
	bool yBounds = (y >= 0 && y < yCount());

	return xBounds && yBounds;
}
