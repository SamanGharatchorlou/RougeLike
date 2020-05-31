#include "pch.h"
#include "AIPathMap.h"

#include "Map/Map.h"


void AIPathMap::clear()
{
	mData.clear();
}


// TODO: many things get use get by reference rather than by value here
void AIPathMap::build(Map* map, int xSplit, int ySplit)
{
	Grid<MapTile> tileMap = map->getData();

	// New larger path tile map
	mData.clear();
	Vector2D<int> size(tileMap.xCount() * xSplit, tileMap.yCount() * ySplit);
	mData.set(size, PathTile());

	mCostMap.set(size, 1);
	occupiedTiles.clear();
	toBeOccupiedTiles.clear();

	// Split each tile into 4
	for (unsigned int y = 0; y < map->yCount(); y++)
	{
		int yPathMap = y * ySplit;

		// Split every y into 2
		for (int yShift = 0; yShift < ySplit; yShift++)
		{
			yPathMap += yShift;

			for (unsigned int x = 0; x < map->xCount(); x++)
			{
				int xPathMap = x * xSplit;

				MapTile mapTile = tileMap[y][x];

				// splt every x into 2
				for (int xShift = 0; xShift < xSplit; xShift++)
				{
					xPathMap += xShift;

					BasicTile::Type type = mapTile.collisionType();
					RectF rect = mapTile.rect();

					VectorF size = rect.Size();
					rect.SetSize(size.x / xSplit, size.y / ySplit);
					
					VectorF topLeft = rect.TopLeft();

					topLeft.x += (rect.Size().x * xShift);
					topLeft.y += (rect.Size().y * yShift);

					rect.SetTopLeft(topLeft);

					PathTile pathTile(type, rect);

					mData[yPathMap][xPathMap] = pathTile;
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
