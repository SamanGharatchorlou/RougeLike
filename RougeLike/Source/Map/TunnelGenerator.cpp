#include "pch.h"
#include "TunnelGenerator.h"


TunnelGenerator::TunnelGenerator()
{
	srand((int)time(NULL));
}


// TODO: make this more dynamic
// i.e. change the frequency of change as the previousChangeIndex moves further away
void TunnelGenerator::buildRandom(Grid<MapTile>& mapData)
{
	int pathWidth = 2;
	int y = mapData.yCount() / 2;

	// Entrace
	mapData[y][0].setType(MapTile::Floor);

	int previousChangeIndex = 0;

	// build tunnel
	for (unsigned int x = 1; x < mapData.xCount(); x++)
	{
		if (previousChangeIndex != x - 1)
		{
			bool change = false;
			int randomNumber = rand() % 100;
			int roughness = 10;

			int maxPathY = 3;
			int minPathY = 1;

			// Change tunnel width
			if (randomNumber > roughness)
			{
				pathWidth += (rand() % maxPathY) / 2;

				if (pathWidth < minPathY)
					pathWidth = minPathY;

				if (pathWidth > maxPathY)
					pathWidth = maxPathY;

				previousChangeIndex = x;
			}

			randomNumber = rand() % 100;
			int curveyness = 50;

			int maxPathChange = 6;

			// Change tunnel direction
			if (randomNumber > 10)
			{
				int yChange = (rand() % (maxPathChange * 2)) - maxPathChange;
				y += yChange;

				if (y <= maxPathY)
					y = maxPathY + minPathY;

				if (y >= mapData.yCount() - maxPathY)
					y = mapData.yCount() - maxPathY - 1 - minPathY;

				previousChangeIndex = x;
			}
		}
		
		for (int i = -pathWidth; i <= pathWidth; i++)
		{
			// Skip over edges of map, make sure its wall
			if (x == 0 || x == mapData.xCount() - 1)
				continue;
			else if (y + i == 0 || y + i == mapData.yCount() - 1)
				continue;

			mapData[y + i][x].setType(MapTile::Floor);
		}
	}

	// Exit
	mapData[mapData.yCount() / 2][mapData.xCount() - 1].setType(MapTile::Floor);
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
	for (; x < map.xCount() - 3; x++)
	{
		// Carve out path
		for (int yPath = y - width / 2; yPath <= y + width / 2; yPath++)
		{
			if (yPath < 0)
				printf("pause");

			Index index(x, yPath);
			map[index].setType(MapTile::Floor);
		}

		// Increment x atleast once before changing the path
		if (x >= previousChange + 1)
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

	int widthA = (map.yCount() / 2) - (y - width / 2);
	int widthB = (map.yCount() / 2) - (y + width / 2);
	width = clamp(std::max(widthA, widthB), 2, map.yCount() - 3);

	y = map.yCount() / 2;

	for (; x < map.xCount(); x++)
	{
		if (x == map.xCount() - 2)
			width = 1;

		for (int yPath = y - width / 2; yPath <= y + width / 2; yPath++)
		{
			Index index(x, yPath);
			map[index].setType(MapTile::Floor);
		}
	}
}



void TunnelGenerator::addExitPath(Grid<MapTile>& mapData, int startingX)
{
	int yPosition = mapData.yCount() / 2;

	// build tunnel
	for (unsigned int x = startingX; x < mapData[0].size(); x++)
	{
		mapData[yPosition][x].setType(MapTile::Floor);
	}
}


void TunnelGenerator::buildSimpleLine(Grid<MapTile>& mapData)
{
	int yPosition = mapData.yCount() / 2;

	// build tunnel
	for (unsigned int x = 0; x < mapData[0].size(); x++)
	{
		mapData[yPosition][x].setType(MapTile::Floor);
	}
}