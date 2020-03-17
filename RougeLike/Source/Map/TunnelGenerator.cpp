#include "pch.h"
#include "TunnelGenerator.h"


TunnelGenerator::TunnelGenerator()
{
	srand((int)time(NULL));
}

void TunnelGenerator::buildRandom(Grid<MapTile>& mapData, int width)
{
	ASSERT(Warning, width <= mapData.xCount(), "Requesting to build a tunnel larger than the map, increase map size\n");

	int pathWidth = 5;
	int y = mapData.yCount() / 2;

	// Entrace
	mapData[y][0].setType(MapTile::Floor);

	// build tunnel
	for (unsigned int x = 1; x < width; x++)
	{
		int randomNumber = rand() % 100;
		int roughness = 20;

		int maxPathY = 4;
		int minPathY = 1;

		// Change tunnel width
		if (randomNumber > roughness)
		{
			pathWidth += (rand() % maxPathY) / 2;

			if (pathWidth < minPathY)
				pathWidth = minPathY;

			if (pathWidth > maxPathY)
				pathWidth = maxPathY;
		}

		randomNumber = rand() % 100;
		int curveyness = 50;

		int maxPathChange = 4;

		// Change tunnel direction
		if (randomNumber > 20)
		{
			int yChange = (rand() % (maxPathChange * 2)) - maxPathChange;
			y += yChange;

			if (y <= maxPathY)
				y = maxPathY + minPathY;

			if (y >= mapData.yCount() - maxPathY)
				y = mapData.yCount() - maxPathY - 1 - minPathY;
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



void TunnelGenerator::addExitPath(Grid<MapTile>& mapData, int startingX)
{
	int yPosition = mapData.yCount() / 2;

	// build tunnel
	for (unsigned int x = startingX; x < mapData[0].size(); x++)
	{
		mapData[yPosition][x].setType(MapTile::Floor);
	}
}


void TunnelGenerator::buildExit(Grid<MapTile>& mapData, int width)
{
	int yPosition = mapData.yCount() / 2;

	// build tunnel
	for (unsigned int x = 0; x < mapData[0].size(); x++)
	{
		mapData[yPosition][x].setType(MapTile::Floor);
	}
}