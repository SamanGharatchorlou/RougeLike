#include "pch.h"
#include "MapTileDecoder.h"

#include "Graphics/TextureManager.h"

#include "RenderTilePopulator.h"
#include "DecorTilePopulator.h"


// temp
#include "System/Files/FileManager.h"
#include "Animations/AnimationReader.h"


void MapTileDecoder::populateData(Grid<MapTile>& data)
{
	RenderTilePopulator renderTiles(mTextureManager);
	renderTiles.populate(data);

	DecorTilePopulator decorTiles(mTextureManager);
	decorTiles.populate(data);

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
	tileTextures[RenderTile::Water_Left] = mTextureManager->getTexture("water_left", FileManager::Image_Maps);
	tileTextures[RenderTile::Water_Top] = mTextureManager->getTexture("water_top", FileManager::Image_Maps);
	tileTextures[RenderTile::Water_Top_Left] = mTextureManager->getTexture("water_top_left", FileManager::Image_Maps);

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

			//if (tile.has(RenderTile::Water_Middle) ||
			if (tile.has(CollisionTile::Water))
				printf("we have water (%d,%d)\n", x,y);

			tile.setTexture(tileTextures[type]);

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
			else if (data[index].has(DecorTile::Water) && renderTile < RenderTile::Water)
			{
				data[index].set(CollisionTile::Floor);
			}
		}
	}
}
