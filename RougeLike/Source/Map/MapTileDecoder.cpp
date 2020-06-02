#include "pch.h"
#include "MapTileDecoder.h"

#include "Graphics/TextureManager.h"




void MapTileDecoder::populateData(Grid<MapTile>& data)
{
	fillRenderInfo(data);
	fillCollisionInfo(data);
}



void MapTileDecoder::fillRenderInfo(Grid<MapTile>& data)
{
	// Label all bottom and top side walls
	topBottom(data);

	// Add right and left labels
	leftRight(data);

	// Add corners & Top and bottom segments
	corners(data);

	// Add corner points
	pointCorners(data);

	// Remove unused labels
	cleanLabels(data);

	setTextures(data);
}


/// --- Private Functions --- ///
bool MapTileDecoder::isValidIndex(Index index, Grid<MapTile>& data) const
{
	return (index.x >= 0 && index.x < data.xCount()) &&
		(index.y >= 0 && index.y < data.yCount());
}


void MapTileDecoder::fillCollisionInfo(Grid<MapTile>& data)
{
	for (int x = 0; x < data.xCount(); x++)
	{
		for (int y = 0; y < data.yCount(); y++)
		{
			Index index(x, y);

			if (data[index].isRenderType(MapTile::Floor))
			{
				data[index].setCollisionType(MapTile::Floor);
			}
			else
			{
				data[index].setCollisionType(MapTile::Wall);
			}

			// In top down view these tiles can be moved 'under'
			if (data[index].hasRenderType(MapTile::Top_Left | MapTile::Top_Upper | MapTile::Top_Right))
			{
				data[index].setCollisionType(MapTile::Floor);
			}
		}
	}
}


void MapTileDecoder::topBottom(Grid<MapTile>& data)
{
	for (int y = 0; y < data.yCount(); y++)
	{
		for (int x = 0; x < data.xCount(); x++)
		{
			Index index(x, y);

			if (data[index].hasRenderType(MapTile::Wall))
			{
				// Botton walls
				Index up = index + Index(0, -1);
				Index down = index + Index(0, +1);

				// Bottom type walls
				if (isValidIndex(down, data) && data[down].hasRenderType(MapTile::Floor))
				{
					// Tile above
					if (isValidIndex(up, data))
						data[up].addRenderType(MapTile::Bottom_Upper);

					data[index].addRenderType(MapTile::Bottom_Lower);
					continue;
				}

				// Top type walls
				if (isValidIndex(up, data) && data[up].hasRenderType(MapTile::Floor))
				{
					// Tile above
					if (isValidIndex(down, data))
						data[down].addRenderType(MapTile::Top_Lower);

					data[index].addRenderType(MapTile::Top_Upper);
					continue;
				}
			}
		}
	}
}
void MapTileDecoder::leftRight(Grid<MapTile>& data)
{
	for (int y = 0; y < data.yCount(); y++)
	{
		for (int x = 0; x < data.xCount(); x++)
		{
			Index index(x, y);
			Index left = index + Index(-1, 0);
			Index right = index + Index(+1, 0);

			// Map Top half
			if (data[index].isRenderType(MapTile::Wall))
			{
				// Map top half
				if (isValidIndex(right, data) && data[right].hasRenderType(MapTile::Floor | MapTile::Bottom_Lower | MapTile::Bottom_Upper))
				{
					data[index].addRenderType(MapTile::Right);
					continue;
				}

				// Unlike the right side as we are moving right we also skip left walls (as we add them)
				if (isValidIndex(left, data) && data[left].hasRenderType(MapTile::Floor | MapTile::Bottom_Lower | MapTile::Bottom_Upper) && !data[left].hasRenderType(MapTile::Left))
				{
					data[index].addRenderType(MapTile::Left);
					continue;
				}
			}

			// Map bot half
			if (data[index].hasRenderType(MapTile::Wall) && data[index].hasRenderType(MapTile::Top_Lower | MapTile::Top_Upper))
			{
				if (isValidIndex(right, data) && data[right].hasRenderType(MapTile::Floor))
				{
					data[index].addRenderType(MapTile::Right);
					continue;
				}

				if (isValidIndex(left, data) && data[left].hasRenderType(MapTile::Floor))
				{
					data[index].addRenderType(MapTile::Left);
					continue;
				}
			}
		}
	}
}
void MapTileDecoder::corners(Grid<MapTile>& data)
{
	for (int y = 0; y < data.yCount(); y++)
	{
		for (int x = 0; x < data.xCount(); x++)
		{
			Index index(x, y);
			Index below = index + Index(0, +1);
			Index above = index + Index(0, -1);

			// Map top half
			if (isValidIndex(below, data) && data[below].hasRenderType(MapTile::Bottom_Upper))
			{
				// Corners
				if (data[index].hasRenderType(MapTile::Left))
				{
					data[index].setRenderType(MapTile::Wall | MapTile::Bottom_Left);
					continue;
				}
				if (data[index].hasRenderType(MapTile::Right))
				{
					data[index].setRenderType(MapTile::Wall | MapTile::Bottom_Right);
					continue;
				}
				// Bottom wall Tops
				if (data[index].isRenderType(MapTile::Wall))
				{
					data[index].setRenderType(MapTile::Wall | MapTile::Bottom);
					continue;
				}
			}

			// Map bot half
			if (isValidIndex(above, data))
			{
				// Corners
				if (data[index].hasRenderType(MapTile::Left))
				{
					if (data[index].hasRenderType(MapTile::Top_Upper))
					{
						data[index].setRenderType(MapTile::Wall | MapTile::Top_Left);
						continue;
					}
				}
				else if (data[index].hasRenderType(MapTile::Right))
				{
					if (data[index].hasRenderType(MapTile::Top_Upper))
					{
						data[index].setRenderType(MapTile::Wall | MapTile::Top_Right);
						continue;
					}
				}
			}

		}
	}
}
void MapTileDecoder::pointCorners(Grid<MapTile>& data)
{
	// points
	for (int y = 0; y < data.yCount(); y++)
	{
		for (int x = 0; x < data.xCount(); x++)
		{
			Index index(x, y);
			Index down = index + Index(0, +1);
			Index left = index + Index(-1, 0);
			Index right = index + Index(+1, 0);

			// Map top half
			if (data[index].isRenderType(MapTile::Wall) && isValidIndex(down, data))
			{
				if (isValidIndex(left, data) &&
					(data[left].hasRenderType(MapTile::Bottom | MapTile::Bottom_Left)) &&
					(data[down].hasRenderType(MapTile::Left | MapTile::Bottom_Left)))
				{
					data[index].addRenderType(MapTile::Point_Bottom_Left);
				}

				if (isValidIndex(right, data) &&
					(data[right].hasRenderType(MapTile::Bottom | MapTile::Bottom_Right)) &&
					(data[down].hasRenderType(MapTile::Right | MapTile::Bottom_Right)))
				{
					data[index].addRenderType(MapTile::Point_Bottom_Right);
				}
			}

			// Map bot half
			if (data[index].hasRenderType(MapTile::Top_Upper | MapTile::Top_Left))
			{
				if (isValidIndex(left, data))
				{
					if (data[left].isRenderType(MapTile::Wall) || data[left].hasRenderType(MapTile::Top_Lower))
					{
						data[left].addRenderType(MapTile::Point_Top_Right);
					}
				}

				if (isValidIndex(right, data))
				{
					if (data[right].isRenderType(MapTile::Wall) || data[right].hasRenderType(MapTile::Top_Lower))
					{
						data[right].addRenderType(MapTile::Point_Top_Left);
					}
				}
			}
		}
	}
}
// Once all the tile types have been set top lower = wall
void MapTileDecoder::cleanLabels(Grid<MapTile>& data)
{
	// points
	for (int y = 0; y < data.yCount(); y++)
	{
		for (int x = 0; x < data.xCount(); x++)
		{
			Index index(x, y);
			data[index].removeRenderType(MapTile::Top_Lower);
		}
	}
}

void MapTileDecoder::setTextures(Grid<MapTile>& data)
{
	std::unordered_map<MapTile::Type, Texture*> tileTextures;
	tileTextures[MapTile::Floor] = mTextureManager->getTexture("floor", FileManager::Image_Maps);
	tileTextures[MapTile::Wall] = mTextureManager->getTexture("wall", FileManager::Image_Maps);
	tileTextures[MapTile::Left] = mTextureManager->getTexture("wall_left", FileManager::Image_Maps);
	tileTextures[MapTile::Right] = mTextureManager->getTexture("wall_right", FileManager::Image_Maps);
	tileTextures[MapTile::Bottom_Lower] = mTextureManager->getTexture("wall_bottom_lower", FileManager::Image_Maps);
	tileTextures[MapTile::Bottom_Upper] = mTextureManager->getTexture("wall_bottom_upper", FileManager::Image_Maps);
	tileTextures[MapTile::Top_Lower] = mTextureManager->getTexture("wall_top_lower", FileManager::Image_Maps);
	tileTextures[MapTile::Top_Upper] = mTextureManager->getTexture("wall_top_upper", FileManager::Image_Maps);
	tileTextures[MapTile::Bottom] = mTextureManager->getTexture("wall_bottom", FileManager::Image_Maps);
	tileTextures[MapTile::Top] = mTextureManager->getTexture("wall_top_upper", FileManager::Image_Maps);
	tileTextures[MapTile::Top_Right] = mTextureManager->getTexture("wall_top_right", FileManager::Image_Maps);
	tileTextures[MapTile::Top_Left] = mTextureManager->getTexture("wall_top_left", FileManager::Image_Maps);
	tileTextures[MapTile::Bottom_Right] = mTextureManager->getTexture("wall_bottom_right", FileManager::Image_Maps);
	tileTextures[MapTile::Bottom_Left] = mTextureManager->getTexture("wall_bottom_left", FileManager::Image_Maps);
	tileTextures[MapTile::Point_Bottom_Right] = mTextureManager->getTexture("point_bottom_right", FileManager::Image_Maps);
	tileTextures[MapTile::Point_Bottom_Left] = mTextureManager->getTexture("point_bottom_left", FileManager::Image_Maps);
	tileTextures[MapTile::Point_Top_Right] = mTextureManager->getTexture("point_top_right", FileManager::Image_Maps);
	tileTextures[MapTile::Point_Top_Left] = mTextureManager->getTexture("point_top_left", FileManager::Image_Maps);

	for (int y = 0; y < data.yCount(); y++)
	{
		for (int x = 0; x < data.xCount(); x++)
		{
			MapTile& tile = data[Index(x, y)];

			MapTile::Type renderType = tile.renderType();

			if (renderType == MapTile::Wall || renderType == MapTile::Floor)
			{
				tile.setTexture(tileTextures[renderType]);
			}
			else
			{
				// Remove wall and floor components
				renderType &= ~(MapTile::Wall | MapTile::Floor);
				tile.setTexture(tileTextures[renderType]);
			}
		}
	}
}