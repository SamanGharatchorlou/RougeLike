#include "pch.h"
#include "PrimaryMap.h"

#include "Game/Camera.h"
#include "MapTile.h"

void PrimaryMap::renderLowerLayer()
{
	Camera* camera = Camera::Get();
	for (unsigned int x = 0; x < xCount(); x++)
	{
		bool onTopSection = true;

		for (unsigned int y = 0; y < yCount(); y++)
		{
			MapTile tile = mData[y][x];

			if (tile.has(RenderTile::Floor))
				onTopSection = false;

			if (!onTopSection)
			{
				// Move to next x
				onTopSection = true;
				break;
			}

			RectF tileRect = tile.rect();

			if (camera->inView(tileRect))
			{
				tileRect = camera->toCameraCoords(tileRect);
				tile.render(tileRect);
			}
		}
	}
}