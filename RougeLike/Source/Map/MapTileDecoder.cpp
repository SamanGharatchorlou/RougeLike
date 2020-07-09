#include "pch.h"
#include "MapTileDecoder.h"

#include "Graphics/TextureManager.h"


// temp
#include "System/Files/FileManager.h"
#include "Animations/AnimationReader.h"


void MapTileDecoder::populateData(Grid<MapTile>& data)
{
	populateRenderInfo(data);
	editCollisionInfo(data);
	populateDecorInfo(data);

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

			tile.setTexture(tileTextures[type]);
		}
	}
}


void MapTileDecoder::populateRenderInfo(Grid<MapTile>& data)
{
	topBottom(data);
	leftRight(data);
	pointCorners(data);
	corners(data);

	cleanRenderLabels(data);
}

void MapTileDecoder::populateDecorInfo(Grid<MapTile>& data)
{
	addWater(data);
	addColumns(data);

	addAnimations(data);
}


// -- Decore Tiles -- //
void MapTileDecoder::addWater(Grid<MapTile>& data)
{
	for (int x = 0; x < data.xCount(); x++)
	{
		for (int y = 0; y < data.yCount(); y++)
		{
			Index index(x, y);

			if (data[index].has(DecorTile::Water))
			{
				// very top row not considered to keep symmetry 
				index += Index(0, 1);

				// width
				int width = 0;
				while (data[index + width].has(DecorTile::Water))
					width++;

				// height
				int height = 0;
				while (data[index + height].has(DecorTile::Water))
					height++;


				// Fill water center
				for (int xPool = index.x + 1; xPool < index.x + width - 1; xPool++)
				{
					for (int yPool = index.y + 1; yPool < index.y + height - 1; yPool++)
					{
						data[Index(xPool, yPool)].set(RenderTile::Water_Middle);
					}
				}

				// Left
				for (int yPool = 0; yPool < height - 1; yPool++)
				{
					data[index + Index(0, yPool)].set(RenderTile::Floor_Right);
				}

				// Right
				for (int yPool = 0; yPool < height - 1; yPool++)
				{
					data[index + Index(width - 1, yPool)].set(RenderTile::Floor_Left);
				}


				// Top
				for (int xPool = 1; xPool < width - 1; xPool++)
				{
					data[index + Index(xPool, 0)].set(RenderTile::Water_Top);
					// Fill very top row
					data[index + Index(xPool, -1)].set(RenderTile::Floor_Bottom);
				}

				// Bot
				for (int xPool = 1; xPool < width - 1; xPool++)
				{
					data[index + Index(xPool, height - 1)].set(RenderTile::Floor_Top);
				}


				// Corners
				data[Index(index.x, index.y - 1)].set(RenderTile::Floor_Bottom_Right);
				data[Index(index.x + width - 1, index.y - 1)].set(RenderTile::Floor_Bottom_Left);
				data[Index(index.x + width - 1, index.y + height - 1)].set(RenderTile::Floor_Top_Left);
				data[Index(index.x, index.y + height - 1)].set(RenderTile::Floor_Top_Right);

				// Move to end of pool + 1
				x = index.x + width + 1;
			}
		}
	}
}
void MapTileDecoder::addColumns(Grid<MapTile>& data)
{
	for (int y = 0; y < data.yCount(); y++)
	{
		for (int x = 0; x < data.xCount(); x++)
		{
			Index index(x, y);

			if (data[index].has(DecorTile::Column))
			{
				if (data[index].has(RenderTile::Bottom))
					data[index].set(RenderTile::Column_Top);

				else if (data[index].has(RenderTile::Bottom_Upper))
					data[index].set(RenderTile::Column_Upper);

				else if (data[index].has(RenderTile::Bottom_Lower))
					data[index].set(RenderTile::Column_Lower);

				else if (data[index].is(CollisionTile::Floor))
					data[index].set(RenderTile::Floor_ColumnBase);
			}
		}
	}
}
void MapTileDecoder::addAnimations(Grid<MapTile>& data)
{
	XMLParser tourchParser(FileManager::Get()->findFile(FileManager::Configs_Objects, "Torch"));
	XMLParser spikeParser(FileManager::Get()->findFile(FileManager::Configs_Objects, "SpikeTrap"));

	for (int x = 0; x < data.xCount(); x++)
	{
		for (int y = 0; y < data.yCount(); y++)
		{
			Index index(x, y);

			if (data[index].has(DecorTile::Torch))
			{
				Animator animator;
				AnimationReader reader(mTextureManager, tourchParser);
				if (reader.initAnimator(animator))
				{
					data[index].addAnimation(animator);
				}
			}

			if (data[index].has(DecorTile::Spikes))
			{
				Animator animator;
				AnimationReader reader(mTextureManager, spikeParser);
				if (reader.initAnimator(animator))
				{
					data[index].addAnimation(animator);
					data[index].animation(0).pause();
				}
			}
		}
	}
}


// --- Collision Tiles --- //
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
			else if (data[index].has(DecorTile::Water) && renderTile > RenderTile::Water)
			{
				data[index].set(CollisionTile::Wall);
			}
		}
	}
}

// -- Render Tiles
void MapTileDecoder::topBottom(Grid<MapTile>& data)
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
void MapTileDecoder::leftRight(Grid<MapTile>& data)
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
void MapTileDecoder::pointCorners(Grid<MapTile>& data)
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
void MapTileDecoder::corners(Grid<MapTile>& data)
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
				if (data[index].is(RenderTile::Wall ^ RenderTile::Left ^ RenderTile::Bottom))
				{
					data[index].set(RenderTile::Bottom_Left);
				}
				if (data[index].is(RenderTile::Wall ^ RenderTile::Right ^ RenderTile::Bottom))
				{
					data[index].set(RenderTile::Bottom_Right);
				}
			}
			else // bottom half
			{
				if (data[index].is(RenderTile::Wall ^ RenderTile::Left ^ RenderTile::Top_Upper))
				{
					data[index].set(RenderTile::Top_Left);
				}
				else if (data[index].is(RenderTile::Wall ^ RenderTile::Right ^ RenderTile::Top_Upper))
				{
					data[index].set(RenderTile::Top_Right);
				
				}
			}
		}
	}
}


void MapTileDecoder::cleanRenderLabels(Grid<MapTile>& data)
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



bool MapTileDecoder::isValid(const Index index, const Grid<MapTile>& data) const
{
	return (index.x >= 0 && index.x < data.xCount()) &&
		(index.y >= 0 && index.y < data.yCount());
}