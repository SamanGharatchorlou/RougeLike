#include "pch.h"
#include "AIPathMap.h"

#include "Map/Map.h"


void AIPathMap::clear()
{
	mData.clear();
}


void AIPathMap::build(const Map* map, int xSplit, int ySplit)
{
	const Grid<MapTile>& tileMap = map->getData();

	// New larger path tile map
	mData.clear();
	const Vector2D<int> size(tileMap.xCount() * xSplit, tileMap.yCount() * ySplit);
	mData.set(size, PathTile());

	mCostMap.set(size, 1);
	occupiedTiles.clear();
	toBeOccupiedTiles.clear();

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

				// splt every x into 2
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
	Index index(shiftedPosition.x / tileSize().x, shiftedPosition.y / tileSize().y);
	return isValidPosition(position) ? index : Index(-1, -1);
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
