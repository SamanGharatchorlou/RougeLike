#pragma once

#include "Utilities/Vector2D.h"

using Index = Vector2D<int>;

template<class K>
class MapBase
{
public:
	MapBase() : mLevel(-1) { }
	MapBase(Vector2D<int> size) : mData(size, K()), mLevel(-1) { }
	MapBase(Vector2D<int> size, K value) : mData(size, value), mLevel(-1) { }
	virtual ~MapBase() { }

	K& operator [] (Index index) { return mData[index.y][index.x]; }
	const K& get(Index index) const { return mData.get(index); }

	const Grid<K>& getData() const { return mData; }
	Grid<K>& getData() { return mData; }

	void clear() { mData.clear(); }

	unsigned int yCount() const { return mData.yCount(); }
	unsigned int xCount() const { return mData.xCount(); }

	bool inBounds(Index index) const;
	bool isValidIndex(Index index) const;

	void setLevel(int level) { mLevel = level; }
	int level() const { return mLevel; }

protected:
	int mLevel;
	Grid<K> mData;
};


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
