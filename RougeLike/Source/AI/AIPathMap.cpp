#include "pch.h"
#include "AIPathMap.h"

#include "Map/Map.h"


void AIPathMap::build(Map* map, int xSplit, int ySplit)
{
	Grid<MapTile> tileMap = map->getData();

	// New larger path tile map
	mData.clear(); // TODO: does this correctly clear the data?
	mData.clearAndSet(tileMap.xCount() * xSplit, tileMap.yCount() * ySplit, PathTile());

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
	return tile(0, 0)->rect().Size();
}


const Index AIPathMap::index(VectorF position) const
{
	VectorF mapTopLeft = mData.get(0, 0).rect().TopLeft();
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
	VectorF start = mData.get(0, 0).rect().TopLeft();
	VectorF end = mData.get(xCount() - 1, yCount() - 1).rect().BotRight();

	bool a = (position.x >= start.x && position.x < end.x) &&
		(position.y >= start.y && position.y < end.y);

	return a;
}


const PathTile* AIPathMap::offsetTile(const PathTile* target, int xOffset, int yOffset) const
{
	Index tileIndex = index(target);
	tileIndex = tileIndex + Index(xOffset, yOffset);

	return isValidIndex(tileIndex) ? tile(tileIndex) : nullptr;
}


void AIPathMap::clearOccupiedTiles() 
{ 
	for (unsigned int i = 0; i < occupiedTiles.size(); i++)
	{
		Index index = occupiedTiles[i];
		mData[index].setOccupied(false);
	}

	occupiedTiles.clear(); 
}


void AIPathMap::addOccupiedTile(VectorF position)
{
	if (isValidPosition(position))
	{
		// Track occupied tiles to make it more efficient when resetting them every frame
		Index tileIndex = index(position);
		occupiedTiles.push_back(tileIndex);

		mData[tileIndex].setOccupied(true);
	}
	else
	{
		DebugPrint(Warning, "Invalid occupied position: %f, %f, not adding to occupied tile list\n", position.x, position.y);
	}
}


void AIPathMap::clearToBeOccupiedTiles()
{
	for (unsigned int i = 0; i < toBeOccupiedTiles.size(); i++)
	{
		Index index = toBeOccupiedTiles[i];
		mData[index].setToBeOccupied(false);
	}

	toBeOccupiedTiles.clear();
}


void AIPathMap::addToBeOccupiedTile(Index index)
{
	if (isValidIndex(index))
	{
		// Track to be occupied tiles to make it more efficient when resetting them every frame
		toBeOccupiedTiles.push_back(index);
		mData[index].setToBeOccupied(true);
	}
	else
	{
		DebugPrint(Warning, "Invalid to be occupied tile index: %d, %d, not adding to the to be occupied tile list\n", index.x, index.y);
	}
}

