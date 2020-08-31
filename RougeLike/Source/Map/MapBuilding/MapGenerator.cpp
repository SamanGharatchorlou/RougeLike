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
	int widthMax = 8;
	int widthMin = 2;

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


