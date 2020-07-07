#include "pch.h"
#include "MapTileDecoder.h"

#include "Graphics/TextureManager.h"




void MapTileDecoder::populateData(Grid<MapTile>& data)
{
	populateRenderInfo(data);
	editCollisionInfo(data);

	setTextures(data);
}


void MapTileDecoder::setTextures(Grid<MapTile>& data)
{
	std::unordered_map<RenderTile, Texture*> tileTextures;
	tileTextures[RenderTile::Floor] = mTextureManager->getTexture("floor", FileManager::Image_Maps);

	tileTextures[RenderTile::Floor_Left] = mTextureManager->getTexture("floor_left", FileManager::Image_Maps);
	tileTextures[RenderTile::Floor_Right] = mTextureManager->getTexture("floor_right", FileManager::Image_Maps);
	tileTextures[RenderTile::Floor_Top] = mTextureManager->getTexture("floor_top", FileManager::Image_Maps);
	tileTextures[RenderTile::Floor_Bottom] = mTextureManager->getTexture("floor_bottom", FileManager::Image_Maps);

	tileTextures[RenderTile::Floor_Top_Right] = mTextureManager->getTexture("floor_top_right", FileManager::Image_Maps);
	tileTextures[RenderTile::Floor_Top_Left] = mTextureManager->getTexture("floor_top_left", FileManager::Image_Maps);
	tileTextures[RenderTile::Floor_Bottom_Right] = mTextureManager->getTexture("floor_bottom_right", FileManager::Image_Maps);
	tileTextures[RenderTile::Floor_Bottom_Left] = mTextureManager->getTexture("floor_bottom_left", FileManager::Image_Maps);

	tileTextures[RenderTile::Water_Middle] = mTextureManager->getTexture("water", FileManager::Image_Maps);
	tileTextures[RenderTile::Water_Top] = mTextureManager->getTexture("water_top", FileManager::Image_Maps);

	tileTextures[RenderTile::Wall] = mTextureManager->getTexture("wall", FileManager::Image_Maps);
	tileTextures[RenderTile::Left] = mTextureManager->getTexture("wall_left", FileManager::Image_Maps);
	tileTextures[RenderTile::Right] = mTextureManager->getTexture("wall_right", FileManager::Image_Maps);
	tileTextures[RenderTile::Bottom_Lower] = mTextureManager->getTexture("wall_bottom_lower", FileManager::Image_Maps);
	tileTextures[RenderTile::Bottom_Upper] = mTextureManager->getTexture("wall_bottom_upper", FileManager::Image_Maps);
	tileTextures[RenderTile::Top_Lower] = mTextureManager->getTexture("wall_top_lower", FileManager::Image_Maps);
	tileTextures[RenderTile::Top_Upper] = mTextureManager->getTexture("wall_top_upper", FileManager::Image_Maps);
	tileTextures[RenderTile::Bottom] = mTextureManager->getTexture("wall_bottom", FileManager::Image_Maps);
	tileTextures[RenderTile::Top] = mTextureManager->getTexture("wall_top_upper", FileManager::Image_Maps);
	tileTextures[RenderTile::Top_Right] = mTextureManager->getTexture("wall_top_right", FileManager::Image_Maps);
	tileTextures[RenderTile::Top_Left] = mTextureManager->getTexture("wall_top_left", FileManager::Image_Maps);
	tileTextures[RenderTile::Bottom_Right] = mTextureManager->getTexture("wall_bottom_right", FileManager::Image_Maps);
	tileTextures[RenderTile::Bottom_Left] = mTextureManager->getTexture("wall_bottom_left", FileManager::Image_Maps);
	tileTextures[RenderTile::Point_Bottom_Right] = mTextureManager->getTexture("point_bottom_right", FileManager::Image_Maps);
	tileTextures[RenderTile::Point_Bottom_Left] = mTextureManager->getTexture("point_bottom_left", FileManager::Image_Maps);
	tileTextures[RenderTile::Point_Top_Right] = mTextureManager->getTexture("point_top_right", FileManager::Image_Maps);
	tileTextures[RenderTile::Point_Top_Left] = mTextureManager->getTexture("point_top_left", FileManager::Image_Maps);

	tileTextures[RenderTile::Floor_ColumnBase] = mTextureManager->getTexture("floor_columnbase", FileManager::Image_Maps);
	tileTextures[RenderTile::Column_Top] = mTextureManager->getTexture("column_top", FileManager::Image_Maps);
	tileTextures[RenderTile::Column_Upper] = mTextureManager->getTexture("column_upper", FileManager::Image_Maps);
	tileTextures[RenderTile::Column_Lower] = mTextureManager->getTexture("column_lower", FileManager::Image_Maps);


	for (int y = 0; y < data.yCount(); y++)
	{
		for (int x = 0; x < data.xCount(); x++)
		{
			MapTile& tile = data[Index(x, y)];
			RenderTile type = tile.renderType();

			if (tile.is(RenderTile::Wall) || tile.is(RenderTile::Floor))
			{
				tile.setTexture(tileTextures[type]);
			}
			else
			{
				// Remove wall and floor components
				type = type & ~(RenderTile::Wall | RenderTile::Floor);
				tile.setTexture(tileTextures[type]);
			}
		}
	}
}


void MapTileDecoder::addWater(Grid<MapTile>& data)
{
	Vector2D<int> poolSize(2, 2);

	for (int y = 0; y < data.yCount(); y++)
	{
		for (int x = 0; x < data.xCount(); x++)
		{
			Index index(x, y);

			if (data[index].is(CollisionTile::Water))
			{
				// Fill water
				for (int xPool = index.x + 1; xPool < index.x + 1 + poolSize.x; xPool++)
				{
					for (int yPool = index.y + 1; yPool < index.y + 1 + poolSize.y; yPool++)
					{
						data[Index(xPool, yPool)].set(RenderTile::Wall | RenderTile::Water_Middle);
					}
				}

				// xPool = left
				int xPoolLeft = index.x;
				for (int yPool = index.y; yPool < index.y + 1 + poolSize.y; yPool++)
				{
					data[Index(xPoolLeft, yPool)].set(RenderTile::Floor | RenderTile::Floor_Right);
				}

				// xPool = right
				int xPoolRight = index.x + 1 + poolSize.x;
				for (int yPool = index.y; yPool < index.y + 1 + poolSize.y; yPool++)
				{
					data[Index(xPoolRight, yPool)].set(RenderTile::Floor | RenderTile::Floor_Left);
				}

				// yPool = top
				int yPoolTop = index.y;
				for (int xPool = index.x + 1; xPool < index.x + 1 + poolSize.x; xPool++)
				{
					data[Index(xPool, yPoolTop)].set(RenderTile::Wall | RenderTile::Water_Top);
					data[Index(xPool, yPoolTop - 1)].set(RenderTile::Floor | RenderTile::Floor_Bottom);
				}

				// yPool = bot
				int yPoolBot = index.y + 1 + poolSize.y;
				for (int xPool = index.x + 1; xPool < index.x + 1 + poolSize.x; xPool++)
				{
					data[Index(xPool, yPoolBot)].set(RenderTile::Floor | RenderTile::Floor_Top);
				}


				// Corners
				data[Index(index.x, index.y - 1)].set(								RenderTile::Floor | RenderTile::Floor_Bottom_Right);
				data[Index(index.x + 1 + poolSize.x, index.y - 1)].set(				RenderTile::Floor | RenderTile::Floor_Bottom_Left);
				data[Index(index.x + 1 + poolSize.x, index.y + 1 + poolSize.y)].set(RenderTile::Floor | RenderTile::Floor_Top_Left);
				data[Index(index.x, index.y + 1 + poolSize.y)].set(					RenderTile::Floor | RenderTile::Floor_Top_Right);
			}
		}
	}
}


void MapTileDecoder::addColumns(Grid<MapTile>& data)
{
	int columnWidth = 3;

	for (int y = 0; y < data.yCount(); y++)
	{
		for (int x = 0; x < data.xCount(); x++)
		{
			Index index(x, y);
			int columnCount = 0;

			if (canBeColumn(data, index))
			{
				// Are there a columnWidth number of available walls in a row?
				for (int columnX = index.x; columnX < index.x + columnWidth; columnX++)
				{
					if (canBeColumn(data, Index(columnX, y)))
					{
						columnCount++;
					}
				}

				if (columnCount == columnWidth)
				{
					Index columnIndex = index + Index(columnWidth/2, 0);

					data[columnIndex + Index(0, -2)].set(RenderTile::Wall  | RenderTile::Column_Top);
					data[columnIndex + Index(0, -1)].set(RenderTile::Wall  | RenderTile::Column_Upper);
					data[columnIndex + Index(0,  0)].set(RenderTile::Wall  | RenderTile::Column_Lower);
					data[columnIndex + Index(0, +1)].set(RenderTile::Floor | RenderTile::Floor_ColumnBase);
				}
			}
		}
	}
}


bool MapTileDecoder::canBeColumn(const Grid<MapTile>& data, const Index lowerIndex) const
{
	bool canBeColumn = false;
	Index index = lowerIndex;

	if (isValidIndex(index, data) && data.get(index).has(RenderTile::Bottom_Lower))
	{
		index = index + Index(0, -1);
		if (isValidIndex(index, data) && data.get(index).has(RenderTile::Bottom_Upper))
		{
			index = index + Index(0, -1);
			if (isValidIndex(index, data) && data.get(index).has(RenderTile::Bottom))
			{
				canBeColumn = true;
			}
		}
	}

	return canBeColumn;
}


void MapTileDecoder::populateRenderInfo(Grid<MapTile>& data)
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

	addWater(data);

}


/// --- Private Functions --- ///
bool MapTileDecoder::isValidIndex(const Index index, const Grid<MapTile>& data) const
{
	return (index.x >= 0 && index.x < data.xCount()) &&
		(index.y >= 0 && index.y < data.yCount());
}


void MapTileDecoder::editCollisionInfo(Grid<MapTile>& data)
{
	for (int x = 0; x < data.xCount(); x++)
	{
		for (int y = 0; y < data.yCount(); y++)
		{
			Index index(x, y);

			RenderTile renderTile = data[index].renderType();

			// In top down view these tiles can be moved 'under'
			if (data[index].has(RenderTile::Top_Left | RenderTile::Top_Upper | RenderTile::Top_Right))
			{
				data[index].set(CollisionTile::Floor);
			}
			else if (renderTile > RenderTile::Water)
			{
				data[index].set(CollisionTile::Wall);
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

			if (data[index].has(RenderTile::Wall))
			{
				// Botton walls
				Index up = index + Index(0, -1);
				Index down = index + Index(0, +1);

				// Bottom type walls
				if (isValidIndex(down, data) && data[down].has(RenderTile::Floor))
				{
					// Tile above
					if (isValidIndex(up, data))
						data[up].add(RenderTile::Bottom_Upper);

					data[index].add(RenderTile::Bottom_Lower);
					continue;
				}

				// Top type walls
				if (isValidIndex(up, data) && data[up].has(RenderTile::Floor))
				{
					// Tile above
					if (isValidIndex(down, data))
						data[down].add(RenderTile::Top_Lower);

					data[index].add(RenderTile::Top_Upper);
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
			if (data[index].is(RenderTile::Wall))
			{
				// Map top half
				if (isValidIndex(right, data) && data[right].has(RenderTile::Floor | RenderTile::Bottom_Lower | RenderTile::Bottom_Upper))
				{
					data[index].add(RenderTile::Right);
					continue;
				}

				// Unlike the right side as we are moving right we also skip left walls (as we add them)
				if (isValidIndex(left, data) && data[left].has(RenderTile::Floor | RenderTile::Bottom_Lower | RenderTile::Bottom_Upper) && !data[left].has(RenderTile::Left))
				{
					data[index].add(RenderTile::Left);
					continue;
				}
			}

			// Map bot half
			if (data[index].has(RenderTile::Wall) && data[index].has(RenderTile::Top_Lower | RenderTile::Top_Upper))
			{
				if (isValidIndex(right, data) && data[right].has(RenderTile::Floor))
				{
					data[index].add(RenderTile::Right);
					continue;
				}

				if (isValidIndex(left, data) && data[left].has(RenderTile::Floor))
				{
					data[index].add(RenderTile::Left);
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
			if (isValidIndex(below, data) && data[below].has(RenderTile::Bottom_Upper))
			{
				// Corners
				if (data[index].has(RenderTile::Left))
				{
					data[index].set(RenderTile::Wall | RenderTile::Bottom_Left);
					continue;
				}
				if (data[index].has(RenderTile::Right))
				{
					data[index].set(RenderTile::Wall | RenderTile::Bottom_Right);
					continue;
				}
				// Bottom wall Tops
				if (data[index].is(RenderTile::Wall))
				{
					data[index].set(RenderTile::Wall | RenderTile::Bottom);
					continue;
				}
			}

			// Map bot half
			if (isValidIndex(above, data))
			{
				// Corners
				if (data[index].has(RenderTile::Left))
				{
					if (data[index].has(RenderTile::Top_Upper))
					{
						data[index].set(RenderTile::Wall | RenderTile::Top_Left);
						continue;
					}
				}
				else if (data[index].has(RenderTile::Right))
				{
					if (data[index].has(RenderTile::Top_Upper))
					{
						data[index].set(RenderTile::Wall | RenderTile::Top_Right);
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
			if (data[index].is(RenderTile::Wall) && isValidIndex(down, data))
			{
				if (isValidIndex(left, data) &&
					(data[left].has(RenderTile::Bottom | RenderTile::Bottom_Left)) &&
					(data[down].has(RenderTile::Left | RenderTile::Bottom_Left)))
				{
					data[index].add(RenderTile::Point_Bottom_Left);
				}

				if (isValidIndex(right, data) &&
					(data[right].has(RenderTile::Bottom | RenderTile::Bottom_Right)) &&
					(data[down].has(RenderTile::Right | RenderTile::Bottom_Right)))
				{
					data[index].add(RenderTile::Point_Bottom_Right);
				}
			}

			// Map bot half
			if (data[index].has(RenderTile::Top_Upper | RenderTile::Top_Left))
			{
				if (isValidIndex(left, data))
				{
					if (data[left].is(RenderTile::Wall) || data[left].has(RenderTile::Top_Lower))
					{
						data[left].add(RenderTile::Point_Top_Right);
					}
				}

				if (isValidIndex(right, data))
				{
					if (data[right].is(RenderTile::Wall) || data[right].has(RenderTile::Top_Lower))
					{
						data[right].add(RenderTile::Point_Top_Left);
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
			data[index].remove(RenderTile::Top_Lower);
		}
	}
}

