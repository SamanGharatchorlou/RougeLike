#include "pch.h"
#include "MapPopulator.h"

#include "Graphics/TextureManager.h"

#include "Map/Map.h"
#include "RenderTilePopulator.h"
#include "DecorTilePopulator.h"
#include "AnimationTilePopulator.h"


// TODO: change the name of this class
void MapPopulator::populateData(Map* map)
{
	Grid<MapTile>& data = map->getData();

	RenderTilePopulator renderTiles;
	renderTiles.fillRenderInfo(data);

	DecorTilePopulator decorTiles;
	decorTiles.fillDecorInfo(data);

	AnimationTilePopulator animationTiles;
	animationTiles.addAnimations(data);

	editCollisionInfo(data);
	setTextures(data);
}


void MapPopulator::setTextures(Grid<MapTile>& data)
{

	const TextureManager* textures = TextureManager::Get();
	std::unordered_map<RenderTile, Texture*> tileTextures;

	tileTextures[RenderTile::Floor] = textures->getTexture("floor", FileManager::Image_Maps);
	tileTextures[RenderTile::Floor_Left] = textures->getTexture("floor_left", FileManager::Image_Maps);
	tileTextures[RenderTile::Floor_Right] = textures->getTexture("floor_right", FileManager::Image_Maps);
	tileTextures[RenderTile::Floor_Top] = textures->getTexture("floor_top", FileManager::Image_Maps);
	tileTextures[RenderTile::Floor_Bottom] = textures->getTexture("floor_bottom", FileManager::Image_Maps);

	tileTextures[RenderTile::Floor_Top_Right] = textures->getTexture("floor_top_right", FileManager::Image_Maps);
	tileTextures[RenderTile::Floor_Top_Left] = textures->getTexture("floor_top_left", FileManager::Image_Maps);
	tileTextures[RenderTile::Floor_Bottom_Right] = textures->getTexture("floor_bottom_right", FileManager::Image_Maps);
	tileTextures[RenderTile::Floor_Bottom_Left] = textures->getTexture("floor_bottom_left", FileManager::Image_Maps);

	tileTextures[RenderTile::Floor_Grating] = textures->getTexture("floor_grating", FileManager::Image_Maps);
	tileTextures[RenderTile::Floor_Grating_Top] = textures->getTexture("floor_grating_top", FileManager::Image_Maps);

	tileTextures[RenderTile::Water_Middle] = textures->getTexture("water", FileManager::Image_Maps);
	tileTextures[RenderTile::Water_Left] = textures->getTexture("water_left", FileManager::Image_Maps);
	tileTextures[RenderTile::Water_Top] = textures->getTexture("water_top", FileManager::Image_Maps);
	tileTextures[RenderTile::Water_Top_Left] = textures->getTexture("water_top_left", FileManager::Image_Maps);

	tileTextures[RenderTile::Wall] = textures->getTexture("wall", FileManager::Image_Maps);
	tileTextures[RenderTile::Left] = textures->getTexture("wall_left", FileManager::Image_Maps);
	tileTextures[RenderTile::Right] = textures->getTexture("wall_right", FileManager::Image_Maps);
	tileTextures[RenderTile::Bottom_Lower] = textures->getTexture("wall_bottom_lower", FileManager::Image_Maps);
	tileTextures[RenderTile::Bottom_Upper] = textures->getTexture("wall_bottom_upper", FileManager::Image_Maps);
	tileTextures[RenderTile::Top_Lower] = textures->getTexture("wall_top_lower", FileManager::Image_Maps);
	tileTextures[RenderTile::Top_Upper] = textures->getTexture("wall_top_upper", FileManager::Image_Maps);
	tileTextures[RenderTile::Bottom] = textures->getTexture("wall_bottom", FileManager::Image_Maps);
	tileTextures[RenderTile::Top] = textures->getTexture("wall_top_upper", FileManager::Image_Maps);
	tileTextures[RenderTile::Top_Right] = textures->getTexture("wall_top_right", FileManager::Image_Maps);
	tileTextures[RenderTile::Top_Left] = textures->getTexture("wall_top_left", FileManager::Image_Maps);
	tileTextures[RenderTile::Bottom_Right] = textures->getTexture("wall_bottom_right", FileManager::Image_Maps);
	tileTextures[RenderTile::Bottom_Left] = textures->getTexture("wall_bottom_left", FileManager::Image_Maps);
	tileTextures[RenderTile::Point_Bottom_Right] = textures->getTexture("point_bottom_right", FileManager::Image_Maps);
	tileTextures[RenderTile::Point_Bottom_Left] = textures->getTexture("point_bottom_left", FileManager::Image_Maps);
	tileTextures[RenderTile::Point_Top_Right] = textures->getTexture("point_top_right", FileManager::Image_Maps);
	tileTextures[RenderTile::Point_Top_Left] = textures->getTexture("point_top_left", FileManager::Image_Maps);

	tileTextures[RenderTile::Floor_ColumnBase] = textures->getTexture("floor_columnbase", FileManager::Image_Maps);
	tileTextures[RenderTile::Column_Top] = textures->getTexture("column_top", FileManager::Image_Maps);
	tileTextures[RenderTile::Column_Upper] = textures->getTexture("column_upper", FileManager::Image_Maps);
	tileTextures[RenderTile::Column_Lower] = textures->getTexture("column_lower", FileManager::Image_Maps);


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



// --- Collision Tiles --- //
void MapPopulator::editCollisionInfo(Grid<MapTile>& data)
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
			else if (data[index].has(DecorType::Water) && renderTile < RenderTile::Water)
			{
				data[index].set(CollisionTile::Floor);
			}
		}
	}
}
