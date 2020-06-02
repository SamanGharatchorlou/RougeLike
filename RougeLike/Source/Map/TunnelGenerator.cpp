#include "pch.h"
#include "TunnelGenerator.h"


TunnelGenerator::TunnelGenerator()
{
	srand((int)time(NULL));
}


void TunnelGenerator::buildRandomA(Grid<MapTile>& map)
{
	int widthMax = 9;
	int widthMin = 1;

	// Entrace values
	int y = map.yCount() / 2;
	int width = 1;

	int previousChange = 0;
	
	int x = 0;
	int mainEnd = map.xCount() - 3;
	for (; x < mainEnd; x++)
	{
		// Carve out path
		for (int yPath = y - width / 2; yPath <= y + width / 2; yPath++)
		{
			Index index(x, yPath);
			map[index].setRenderType(MapTile::Floor);
		}

		// Increment x atleast once before changing the path
		if (x >= previousChange + 1 && x < mainEnd - 3 )
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

				y = clamp(y, 1 + nextWidth / 2, map.yCount() - 2 - (nextWidth / 2));

				// Top most and bottom most tiles of the map must be walls
				if (y - width / 2 <= 1 || y + width / 2 >= map.yCount() - 2)
					continue;

				validPath = true;
				previousChange = x + 1;
				width = nextWidth;
			}
		}
	}


	// Exit
	float halfWidth = std::abs(y - (map.yCount() / 2)) + 1;
	width = halfWidth * 2;

	y = map.yCount() / 2;

	for (; x < map.xCount(); x++)
	{
		if (x == map.xCount() - 2)
			width = clamp(width, 0, 2);
		else if (x == map.xCount() - 1)
			width = 1;

		for (int yPath = y - width / 2; yPath <= y + width / 2; yPath++)
		{
			Index index(x, yPath);
			map[index].setRenderType(MapTile::Floor);
		}
	}
}


void TunnelGenerator::buildSimpleLine(Grid<MapTile>& mapData)
{
	int yPosition = mapData.yCount() / 2;

	// build tunnel
	for (unsigned int x = 0; x < mapData[0].size(); x++)
	{
		mapData[yPosition][x].setRenderType(MapTile::Floor);
	}
}