#include "pch.h"
#include "MapGenerator.h"


MapGenerator::MapGenerator()
{
	srand((int)time(NULL));
}


void MapGenerator::prepareMap(Grid<MapTile>& data)
{
	for (int y = 0; y < data.yCount(); y++)
	{
		for (int x = 0; x < data.xCount(); x++)
		{
			data[Index(x, y)].set(CollisionTile::Wall);
		}
	}
}


void MapGenerator::buildDungeon(Grid<MapTile>& data)
{
	prepareMap(data);

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
		if (x >= previousChange + 1 && x < mainEnd - 3)
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


	// Exit
	float halfWidth = std::abs(y - (data.yCount() / 2)) + 1;
	width = halfWidth * 2;

	y = data.yCount() / 2;

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

	completeBuild(data);
}


void MapGenerator::buildCorridor(Grid<MapTile>& data)
{
	prepareMap(data);

	int yPosition = data.yCount() / 2;

	// build tunnel
	for (unsigned int x = 0; x < data[0].size(); x++)
	{
		data[Index(x,yPosition)].set(CollisionTile::Floor);
	}

	completeBuild(data);
}


void MapGenerator::completeBuild(Grid<MapTile>& data)
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


void MapGenerator::addWaterFeatures(Grid<MapTile>& data)
{
	Vector2D<int> poolSize(2, 2);

	int minPoolSpace = data.xCount() / 4;
	int xWaterIndex = -minPoolSpace;

	for (int y = 0; y < data.yCount(); y++)
	{
		for (int x = 0; x < data.xCount(); x++)
		{
			Index index(x, y);

			if (canAddWater(data, index, poolSize))
			{
				printf("can add water at %d\n", x);
				printf("xWaterIndex: %d || minPoolSpace: %d\n", xWaterIndex, minPoolSpace);
			}


			if (x > xWaterIndex + minPoolSpace && canAddWater(data, index, poolSize))
			{
				xWaterIndex = x;
				data[Index(x, y)].set(CollisionTile::Water);
				printf("adding water at index(%d)\n", xWaterIndex);

				//// +1 either side or pool size
				//for (int x = index.x; x < index.x + poolSize.x + 2; x++)
				//{
				//	// +2 on top, + 1 on bot
				//	for (int y = index.y - 1; y < index.y + poolSize.y + 2; y++)
				//	{
				//		data[Index(x, y)].set(CollisionTile::Water);
				//	}
				//}
			}
		}
	}
}



bool MapGenerator::canAddWater(const Grid<MapTile>& data, const Index index, Vector2D<int> size) const
{
	// +1 either side or pool size
	for (int x = index.x; x < index.x + size.x + 2; x++)
	{
		// +2 on top, + 1 on bot
		for (int y = index.y - 1; y < index.y + size.y + 2; y++)
		{
			if (!isValidIndex(Index(x, y), data))
				return false;

			if (!data.get(Index(x, y)).is(CollisionTile::Floor))
				return false;
		}
	}

	return true;
}


bool MapGenerator::isValidIndex(const Index index, const Grid<MapTile>& data) const
{
	return (index.x >= 0 && index.x < data.xCount()) &&
		(index.y >= 0 && index.y < data.yCount());
}