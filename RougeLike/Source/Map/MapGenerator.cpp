#include "pch.h"
#include "MapGenerator.h"


MapGenerator::MapGenerator()
{
	srand((int)time(NULL));
}




void MapGenerator::buildDungeon(Grid<MapTile>& data)
{
	fillWithDefaultTiles(data);

	Index endIndex;
	carveRandomTunnel(data, endIndex);
	addExitOpening(data, endIndex);
	setRenderTiles(data);
}


void MapGenerator::buildCorridor(Grid<MapTile>& data)
{
	fillWithDefaultTiles(data);
	carveStraightTunnel(data);
	setRenderTiles(data);
}


void MapGenerator::fillWithDefaultTiles(Grid<MapTile>& data)
{
	for (int y = 0; y < data.yCount(); y++)
	{
		for (int x = 0; x < data.xCount(); x++)
		{
			data[Index(x, y)].set(CollisionTile::Wall);
		}
	}
}


void MapGenerator::carveRandomTunnel(Grid<MapTile>& data, Index& outEndIndex)
{
	int widthMax = 9;
	int widthMin = 1;

	// Entrace values
	int y = data.yCount() / 2;
	int width = 1;

	int previousChange = 0;

	int x = 0;
	int mainEnd = data.xCount() - 3;
	for (; x < mainEnd; x++)
	{
		// Carve out path
		for (int yPath = y - width / 2; yPath <= y + width / 2; yPath++)
		{
			data[Index(x, yPath)].set(CollisionTile::Floor);
		}

		// Increment x atleast once before changing the path
		if (x >= previousChange + 2 && x < mainEnd - 3)
		{
			bool validPath = false;

			while (!validPath)
			{
				// get random width
				int nextWidth = randomNumberBetween(widthMin, widthMax + 1);

				// Has to be odd
				if (nextWidth % 2 == 0)
					continue;

				y = randomNumberBetween(y - nextWidth / 2, y + (nextWidth / 2) + 1);

				y = clamp(y, 1 + nextWidth / 2, data.yCount() - 2 - (nextWidth / 2));

				// Top most and bottom most tiles of the map must be walls
				if (y - width / 2 <= 1 || y + width / 2 >= data.yCount() - 2)
					continue;

				validPath = true;
				previousChange = x + 1;
				width = nextWidth;
			}
		}
	}

	outEndIndex = Index(x, y);

	// add wall barriers top and bottom
	int yIndexs[4] = { 0, 1, data.yCount() - 2, data.yCount() - 1 };
	for (int j = 0; j < 4; j++)
	{
		for (int i = 0; i < data.xCount(); i++)
		{
			data[Index(i, yIndexs[j])].set(CollisionTile::Wall);
		}
	}
}


void MapGenerator::addExitOpening(Grid<MapTile>& data, Index opening)
{
	int x = opening.x;
	int y = data.yCount() / 2;
	int width = (std::abs(opening.y - (data.yCount() / 2)) + 1) * 2;

	for (; x < data.xCount(); x++)
	{
		if (x == data.xCount() - 2)
			width = clamp(width, 0, 2);
		else if (x == data.xCount() - 1)
			width = 1;

		for (int yPath = y - width / 2; yPath <= y + width / 2; yPath++)
		{
			data[Index(x, yPath)].set(CollisionTile::Floor);
		}
	}
}




void MapGenerator::carveStraightTunnel(Grid<MapTile>& data)
{
	int yPosition = data.yCount() / 2;

	// build tunnel
	for (unsigned int x = 0; x < data[0].size(); x++)
	{
		data[Index(x, yPosition)].set(CollisionTile::Floor);
	}
}


void MapGenerator::setRenderTiles(Grid<MapTile>& data)
{
	for (int y = 0; y < data.yCount(); y++)
	{
		for (int x = 0; x < data.xCount(); x++)
		{
			if (data[Index(x, y)].is(CollisionTile::Wall))
			{
				data[Index(x, y)].set(RenderTile::Wall);
			}
			else if (data[Index(x, y)].is(CollisionTile::Floor))
			{
				data[Index(x, y)].set(RenderTile::Floor);
			}
		}
	}
}


// Water
void MapGenerator::addWaterFeatures(Grid<MapTile>& data)
{
	Vector2D<int> poolSize(2, 2);

	int minPoolSpace = data.xCount() / 4;
	int xWaterIndex = -minPoolSpace;

	for (int x = poolSize.x + 1; x < data.xCount() - poolSize.x - 1; x++)
	{
		for (int y = poolSize.y + 3; y < data.yCount() - poolSize.y - 2; y++)
		{
			Index index(x, y);

			if (x > xWaterIndex + minPoolSpace && canAddWater(data, index, poolSize))
			{
				xWaterIndex = x;

				// +1 either sides of pool size
				for (int x = index.x; x < index.x + poolSize.x + 2; x++)
				{
					for (int y = index.y; y < index.y + poolSize.y + 3; y++)
					{
						data[Index(x, y)].add(DecorTile::Water);
					}
				}
			}
		}
	}
}


bool MapGenerator::canAddWater(const Grid<MapTile>& data, Index index, Vector2D<int> size) const
{
	// +1 either sides of pool
	for (int x = index.x; x < index.x + size.x + 2; x++)
	{
		// +3 (two water tiles + one empty tile for column base) on top + 1 on bottom
		for (int y = index.y - 1; y < index.y + size.y + 3; y++)
		{
			if (!data.get(Index(x, y)).is(CollisionTile::Floor))
				return false;
		}
	}

	return true;
}


// Columns
void MapGenerator::addColumns(Grid<MapTile>& data)
{
	int columnWidth = 3;

	for (int x = 0; x < data.xCount(); x++)
	{
		for (int y = 0; y < data.yCount(); y++)
		{
			Index index(x, y);
			Index down(index + Index(0, 1));

			bool isLowerWall = data[index].is(CollisionTile::Wall) && isValid(down, data) && data[down].is(CollisionTile::Floor);

			if (isLowerWall && canAddColumn(data, index, columnWidth))
			{
				Index up(index + Index(0, -1));
				Index up2(index + Index(0, -2));

				if(isValid(up2, data))
					data[up2].add(DecorTile::Column); // Wall top

				if(isValid(up, data))
					data[up].add(DecorTile::Column); // Upper

				data[index].add(DecorTile::Column); // Lower
				data[down].add(DecorTile::Column); // Base

				x += columnWidth - 1;
				break;
			}
		}
	}
}


bool MapGenerator::canAddColumn(const Grid<MapTile>& data, Index lowerIndex, int columnWidth) const
{
	for (int i = lowerIndex.x - (columnWidth / 2); i <= lowerIndex.x + (columnWidth / 2); i++)
	{
		Index index(i, lowerIndex.y);
		Index down(index + Index(0, 1));

		bool validIndexes = isValid(index, data) && isValid(down, data);
		bool isLowerWall = validIndexes && data.get(index).is(CollisionTile::Wall) && data.get(down).is(CollisionTile::Floor);

		if (!isLowerWall)
			return false;
	}

	return true;
}


// Torches
void MapGenerator::addTorchHandles(Grid<MapTile>& data)
{
	for (int x = 0; x < data.xCount(); x++)
	{
		for (int y = 0; y < data.yCount(); y++)
		{
			Index index(x, y);
			Index left(index + Index(-1, 0));
			Index down(index + Index(0, 1));
			Index down2(index + Index(0, 2));

			if (isValid(left, data) && isValid(down2, data) && isValid(down2, data))
			{
				if (data[left].has(DecorTile::Column) && data[down].is(CollisionTile::Wall) && data[down2].is(CollisionTile::Floor))
				{
					data[down].add(DecorTile::Torch_Handle);
				}
			}
		}
	}
}


// Spikes
void MapGenerator::addSpikes(Grid<MapTile>& data)
{
	for (int x = 0; x < data.xCount(); x++)
	{
		for (int y = 0; y < data.yCount(); y++)
		{
			Index index(x, y);
			Index up(x, y);

			if (data[index].is(CollisionTile::Floor) && data[index].is(DecorTile::None))
			{
				bool addSpike = randomNumberBetween(0, 101) > 85;
				if (addSpike)
				{
					data[Index(x, y)].add(DecorTile::Spikes);
				}
			}
		}
	}
}


bool MapGenerator::isValid(const Index index, const Grid<MapTile>& data) const
{
	return (index.x >= 0 && index.x < data.xCount()) &&
		(index.y >= 0 && index.y < data.yCount());
}