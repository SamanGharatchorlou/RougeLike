#include "pch.h"
#include "AIPathMap.h"

#include "Map/Map.h"


void AIPathMap::clear()
{
	mData.clear();
}


void AIPathMap::build(const Map* map, int xSplit, int ySplit)
{
	setLevel(map->level());

	const Grid<MapTile>& tileMap = map->getData();

	// New larger path tile map
	mData.clear();
	const Vector2D<int> size(tileMap.xCount() * xSplit, tileMap.yCount() * ySplit);
	mData.set(size, PathTile());

	mCostMap.set(size, 1);

	// Split each tile by xsplit and ysplit
	for (unsigned int y = 0; y < map->yCount(); y++)
	{
		int yPathMap = y * ySplit;

		for (int yShift = 0; yShift < ySplit; yShift++)
		{
			int yIndex = yPathMap + yShift;

			for (unsigned int x = 0; x < map->xCount(); x++)
			{
				int xPathMap = x * xSplit;

				const MapTile& mapTile = tileMap.get(Index(x, y));

				for (int xShift = 0; xShift < xSplit; xShift++)
				{
					int xIndex = xPathMap + xShift;
					CollisionTile type = mapTile.collisionType();
					RectF rect = mapTile.rect();

					VectorF size = rect.Size();
					rect.SetSize(size.x / xSplit, size.y / ySplit);
					
					VectorF topLeft = rect.TopLeft();
					topLeft.x += (rect.Size().x * xShift);
					topLeft.y += (rect.Size().y * yShift);
					rect.SetTopLeft(topLeft);

					mData[Index(xIndex, yIndex)] = PathTile(type, rect);
				}
			}
		}
	}
}

const VectorF AIPathMap::tileSize() const
{
	return tile(Index(0,0))->rect().Size();
}


const Index AIPathMap::index(VectorF position) const
{
	VectorF mapTopLeft = mData.get(Index(0, 0)).rect().TopLeft();
	VectorF shiftedPosition = position - mapTopLeft;

	// Get the index relative to this map
	VectorF relativeIndex = shiftedPosition / tileSize();
	return isValidPosition(position) ? relativeIndex.toInt() : Index(-1, -1);
}


const Index AIPathMap::index(const PathTile* tile) const
{
	VectorF position(tile->rect().TopLeft());
	return index(position);
}


bool AIPathMap::isValidPosition(VectorF position) const
{
	VectorF start = mData.get(Index(0,0)).rect().TopLeft();
	VectorF end = mData.get(Index(xCount(), yCount()) - 1).rect().BotRight();

	return (position.x >= start.x && position.x < end.x) &&
		(position.y >= start.y && position.y < end.y);
}


const PathTile* AIPathMap::offsetTile(const PathTile* target, int xOffset, int yOffset) const
{
	Index tileIndex = index(target) + Index(xOffset, yOffset);
	return isValidIndex(tileIndex) ? tile(tileIndex) : nullptr;
}



Vector2D<int> AIPathMap::yTileFloorRange(Index index) const
{
	// This assert is annoying, turn it off
	//ASSERT(Warning, tile(index)->is(CollisionTile::Floor), "Not a floor tile, cannot get yTile floor range");

	Index bottomIndex(index);
	while (true)
	{
		Index increment(0, 1);
		if (!tile(bottomIndex + increment)->is(CollisionTile::Floor))
			break;

		bottomIndex += increment;
	}

	Index topIndex(index);
	while (true)
	{
		Index increment(0, -1);
		if (!tile(topIndex + increment)->is(CollisionTile::Floor))
			break;

		topIndex += increment;
	}

	return Vector2D<int>(topIndex.y, bottomIndex.y);
}


const PathTile* AIPathMap::randomFloorTile(int xPointPercentage) const
{
	int xTileIndex = (int)((xCount() * xPointPercentage) / 100);

	std::vector<Index> floorIndexes;
	for (int y = 0; y < yCount(); y++)
	{
		Index index(xTileIndex, y);
		if (tile(index)->is(CollisionTile::Floor))
			floorIndexes.push_back(index);
	}

	// TODO: -1 -> we dont want to include the 'hidden' floor under the parralax walls?
	int randomIndex = randomNumberBetween(0, floorIndexes.size());
	Index index(floorIndexes[randomIndex]);
	return tile(index);
}


const PathTile* AIPathMap::randomFloorTile() const
{
	int randomXTileIndex = randomNumberBetween(0, xCount());

	std::vector<Index> floorIndexes;
	for (int y = 0; y < yCount(); y++)
	{
		Index index(randomXTileIndex, y);
		if (tile(index)->is(CollisionTile::Floor))
			floorIndexes.push_back(index);
	}

	// TODO: -1 -> we dont want to include the 'hidden' floor under the parralax walls?
	int randomIndex = randomNumberBetween(0, floorIndexes.size());
	Index index(floorIndexes[randomIndex]);
	return tile(index);
}