#pragma once

#include "Utilities/Vector2D.h"
using Index = Vector2D<int>;

template<class T>
class Grid
{
public:
	Grid() { }
	Grid(unsigned int y, unsigned int x, T value = 0);

	// TODO: cannot make const?
	std::vector<T>& operator [] (int y);
	T& operator [] (Vector2D<int> index);

	void clear();
	void clearAndSet(Index index, T value);

	const std::vector<std::vector<T>>& get() const { return data; }

	const int rows() const { return data.size(); }
	const int colums() const { return data[0].size(); }
	const int yCount() const { return data.size(); }
	const int xCount() const { return data[0].size(); }

	// Getters
	const T& get(Index index) const { return data[index.y][index.x]; }

	const std::vector<T> getRow( int y) const;
	const std::vector<T> getColumn(int x) const;

	bool inBounds(Vector2D<int> index) const 
	{
		return index.x >= 0 && index.x < xCount() &&
			index.y >= 0 && index.y < yCount();
	}

#if _DEBUG
	void printBinaryMap(T zero, T one) const;
	void printBinaryMap(T zero) const;
	void printMap() const;
#endif

private:
	std::vector<std::vector<T>> data;
};


template<class T>
Grid<T>::Grid(unsigned int y, unsigned int x, T value)
{
	ASSERT(Error, y > 0 && x > 0,
		"Grid must have at least 1 column and 1 row, set with %d columns, %d rows\n", y, x);

	std::vector<T> row(x, value);
	data = std::vector<std::vector<T>>(y, row);
}


template<class T>
std::vector<T>& Grid<T>::operator [] (int y)
{
#if _DEBUG
	if (y >= 0 && y < yCount() == false)
		DebugPrint(Error, "Attempting to get out of bounds row %d. Data only has %d rows\n", y, yCount());
#endif

	ASSERT(Error, y >= 0 && y < yCount(),
		"Attempting to get out of bounds row %d. Data only has %d rows\n", y, yCount());

	return data[y];
}

template<class T>
T& Grid<T>::operator [] (Vector2D<int> index)
{
	ASSERT(Error, inBounds(index),
		"Attempting to get out of bounds index %d, %d in data of size %d, %d \n",
		index.x, index.y, xCount(), yCount());

	return data[index.y][index.x];
}


template<class T>
const std::vector<T> Grid<T>::getRow(int y) const
{
	ASSERT(Error, y >= 0 && y < yCount(),
		"Attempting to get out of bounds row %d. Data only has %d rows\n", y, yCount());

	return data[y];
}


template<class T>
const std::vector<T> Grid<T>::getColumn(int x) const
{
	ASSERT(Warning, x >= 0 && x < xCount(),
		"Attempting to get out of bounds column %d. Data only has %d columns\n", x, xCount());

	std::vector<T> column;
	column.reserve(data.size());

	for (unsigned int i = 0; i < yCount(); i++)
	{
		column.push_back(data[i][x]);
	}

	return column;
}


template<class T>
void Grid<T>::clearAndSet(Index index, T value)
{
	clear();

	ASSERT(Warning, index.y > 0 && index.x > 0,
		"Grid must have at least 1 column and 1 row, set with %d columns, %d rows\n", index.y, index.x);

	std::vector<T> row(index.x, value);
	for(int i = 0; i < index.y; i++ )
		data.push_back(row);
}


template<class T>
void Grid<T>::clear()
{
	data.clear();
}



// --- Debugging --- //
#if _DEBUG

template<class T>
void Grid<T>::printBinaryMap(T zero, T one) const
{
	for (int y = 0; y < yCount(); y++)
	{
		for (int x = 0; x < xCount(); x++)
		{
			if (data[y][x] == zero)
				DebugPrint(Log, "0 ");
			else if (data[y][x] == one)
				DebugPrint(Log, "1 ");

		}

		DebugPrint(Log, "\n");
	}
}


template<class T>
void Grid<T>::printBinaryMap(T zero) const
{
	for (int y = 0; y < yCount(); y++)
	{
		for (int x = 0; x < xCount(); x++)
		{
			if (data[y][x] == zero)
				DebugPrint(Log, "0 ");
			else
				DebugPrint(Log, "1 ");

		}

		DebugPrint(Log, "\n");
	}
}


template<class T>
void Grid<T>::printMap() const
{
	for (int y = 0; y < yCount(); y++)
	{
		for (int x = 0; x < xCount(); x++)
		{
			DebugPrint(Log, "%d ", (int)data[y][x]);
		}

		DebugPrint(Log, "\n");
	}
}
#endif