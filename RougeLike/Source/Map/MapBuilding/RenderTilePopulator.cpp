#include "pch.h"
#include "RenderTilePopulator.h"

#include "Map/Tiles/MapTile.h"


void RenderTilePopulator::fillRenderInfo(Grid<MapTile>& data)
{
	topBottom(data);
	leftRight(data);
	pointCorners(data);
	corners(data);

	cleanRenderLabels(data);
}


void RenderTilePopulator::topBottom(Grid<MapTile>& data)
{
	for (int x = 0; x < data.xCount(); x++)
	{
		bool isTopHalf = true;
		for (int y = 0; y < data.yCount(); y++)
		{
			Index index(x, y);

			if (isTopHalf && data[index].is(CollisionTile::Floor))
				isTopHalf = false;

			if (data[index].is(CollisionTile::Wall))
			{
				Index up = index + Index(0, -1);
				Index up2 = index + Index(0, -2);
				Index down = index + Index(0, +1);

				if (isTopHalf)
				{
					if (isValid(down, data) && data[down].is(CollisionTile::Floor))
					{
						data[index].set(RenderTile::Bottom_Lower);

						if (isValid(up, data))
							data[up].set(RenderTile::Bottom_Upper);

						if (isValid(up2, data))
							data[up2].add(RenderTile::Bottom);
					}
				}
				else // bottom half
				{
					if (isValid(up, data) && data[up].is(CollisionTile::Floor))
					{
						data[index].add(RenderTile::Top_Upper);

						if (isValid(down, data))
							data[down].add(RenderTile::Top_Lower);
					}
				}
			}
		}
	}
}
void RenderTilePopulator::leftRight(Grid<MapTile>& data)
{
	for (int x = 0; x < data.xCount(); x++)
	{
		bool isTopHalf = true;
		for (int y = 0; y < data.yCount(); y++)
		{
			Index index(x, y);
			Index left = index + Index(-1, 0);
			Index right = index + Index(+1, 0);

			if (isTopHalf && data[index].is(CollisionTile::Floor))
				isTopHalf = false;

			if (isTopHalf)
			{
				if (data[index].has(RenderTile::Wall))
				{
					if (isValid(right, data) && !data[right].has(RenderTile::Wall))
					{
						data[index].add(RenderTile::Right);
					}
					else if (isValid(left, data) && !data[left].has(RenderTile::Wall))
					{
						data[index].add(RenderTile::Left);
					}
				}
			}
			else // bottom half
			{
				if (data[index].has(RenderTile::Wall))
				{
					if (isValid(right, data) && data[right].has(RenderTile::Floor))
					{
						data[index].add(RenderTile::Right);
					}

					if (isValid(left, data) && data[left].has(RenderTile::Floor))
					{
						data[index].add(RenderTile::Left);
					}
				}
			}
		}
	}
}
// Point corner error
// For the first map the top right point corner cannot be set because it
// never gets set as being the bottom half
void RenderTilePopulator::pointCorners(Grid<MapTile>& data)
{
	for (int x = 0; x < data.xCount(); x++)
	{
		bool isTopHalf = true;
		for (int y = 0; y < data.yCount(); y++)
		{
			Index index(x, y);
			Index up = index + Index(0, -1);
			Index down = index + Index(0, +1);
			Index left = index + Index(-1, 0);
			Index right = index + Index(+1, 0);

			if (isTopHalf && data[index].is(CollisionTile::Floor))
				isTopHalf = false;

			if (isTopHalf)
			{
				if (isValid(down, data) && isValid(left, data))
				{
					if ((data[left].has(RenderTile::Bottom)) && data[down].has(RenderTile::Left))
					{
						data[index].set(RenderTile::Point_Bottom_Left);
					}
				}

				if (isValid(down, data) && isValid(right, data))
				{
					if ((data[right].has(RenderTile::Bottom)) && data[down].has(RenderTile::Right))
					{
						data[index].set(RenderTile::Point_Bottom_Right);
					}
				}
			}
			else // bottom half
			{
				if (isValid(right, data) && isValid(up, data))
				{
					if (data[right].has(RenderTile::Top_Upper) && data[up].has(RenderTile::Right))
					{
						data[index].set(RenderTile::Point_Top_Right);
					}
				}

				if (isValid(left, data) && isValid(up, data))
				{
					if (data[left].has(RenderTile::Top_Upper) && data[up].has(RenderTile::Left))
					{
						data[index].set(RenderTile::Point_Top_Left);
					}
				}
			}
		}
	}
}
void RenderTilePopulator::corners(Grid<MapTile>& data)
{
	for (int x = 0; x < data.xCount(); x++)
	{
		bool isTopHalf = true;
		for (int y = 0; y < data.yCount(); y++)
		{
			Index index(x, y);

			if (isTopHalf && data[index].is(CollisionTile::Floor))
				isTopHalf = false;

			if (isTopHalf)
			{
				if (data[index].is(RenderTile::Wall | RenderTile::Left | RenderTile::Bottom))
				{
					data[index].set(RenderTile::Bottom_Left);
				}
				if (data[index].is(RenderTile::Wall | RenderTile::Right | RenderTile::Bottom))
				{
					data[index].set(RenderTile::Bottom_Right);
				}
			}
			else // bottom half
			{
				if (data[index].is(RenderTile::Wall | RenderTile::Left | RenderTile::Top_Upper))
				{
					data[index].set(RenderTile::Top_Left);
				}
				else if (data[index].is(RenderTile::Wall | RenderTile::Right | RenderTile::Top_Upper))
				{
					data[index].set(RenderTile::Top_Right);

				}
			}
		}
	}
}


void RenderTilePopulator::cleanRenderLabels(Grid<MapTile>& data)
{
	// points
	for (int y = 0; y < data.yCount(); y++)
	{
		for (int x = 0; x < data.xCount(); x++)
		{
			Index index(x, y);

			// This is just a basic wall tile
			data[index].remove(RenderTile::Top_Lower);

			if (!data[index].is(RenderTile::Wall))
			{
				data[index].remove(RenderTile::Wall);
			}
		}
	}
}
