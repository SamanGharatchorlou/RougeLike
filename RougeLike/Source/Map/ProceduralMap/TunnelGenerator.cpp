#include "pch.h"
#include "TunnelGenerator.h"


TunnelGenerator::TunnelGenerator(Grid<MapTile>& mapData) : map(mapData)
{
	srand((int)time(NULL));
}


void TunnelGenerator::build()
{
	int pathWidth = 5;
	int yPath = map.yCount() / 2;
	int y = yPath;

	// creat first entry point
	ASSERT(Log, pathWidth > 1, 
		"The initial pathWidth(%d) must be bigger than 1 to create a starting section\n", pathWidth);

	for (int z = 0; z < 3; z++)
	{
		for (int yStart = yPath - pathWidth / 2; yStart < y + pathWidth / 2; yStart++)
		{
			//map[yStart][0].setRenderType(MapTile::Floor);
			map[yStart][z].setType(MapTile::Floor);
		}
	}


	// build tunnel
	for (unsigned int x = 1; x < map[0].size(); x++)
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

			if (y >= map.yCount() - maxPathY)
				y = map.yCount() - maxPathY - 1 - minPathY;
		}
		
		for (int i = -pathWidth; i <= pathWidth; i++)
		{
			// Skip over edges of map, make sure its wall
			if (x == 0 || x == map.xCount() - 1)
				continue;
			else if (y + i == 0 || y + i == map.yCount() - 1)
				continue;

			//map[y + i][x].setRenderType(MapTile::Floor);
			map[y + i][x].setType(MapTile::Floor);
		}
	}
}
