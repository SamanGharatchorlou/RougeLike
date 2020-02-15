#include "pch.h"

#include "Graphics/RenderManager.h"

#include "Managers/GameController.h"

#include "UI/UIManager.h"

#include "Characters/Player/Player.h"
#include "Characters/Enemies/EnemyManager.h"
#include "Characters/Enemies/Enemy.h"

#include "Environment/MapDecoder.h"
#include "Environment/LevelMapping.h"

#include "Game/Camera.h"
#include "Game/Cursor.h"


float PlayerRenderer::getYPosition()
{
	return player->getMovement().getPostion().y;
}

void PlayerRenderer::render()
{
	player->render();
}


// populates map with data
// TODO: may/may not take a while, set a loading screen here?
void RenderManager::initMap(std::string mapDataFilePath)
{
	MapDecoder mapDecoder(mGameData->textureManager, &mMap);
	mapDecoder.decodeMap(mapDataFilePath);

	// TODO: THIS SHOULD NOT BE HERE!
	// need some kind of Map object that is in game data?
	mGameData->mapDimentions = mMap.level->getMapDimentions();
}


void RenderManager::render()
{
	Camera camera = *mGameData->camera;

	// not taking into account the screen!
	// get player y tile
	int yPlayerTile = mPlayer.getYPosition() / mMap.level->getTileDimentions().y; // <- round this

	//for (int i = 0; i < mMap.level->getLayerCount(); i++)
	//{
	//	const std::vector<std::vector<int>> levelMapping = mMap.level->getLayer(i).getMapping();

	//	for (int y = 0; y < levelMapping.size(); y++)
	//	{
	//		for (int x = 0; x < levelMapping[0].size(); x++)
	//		{
	//			int mapping = levelMapping[y][x];

	//			// no tile, do not render
	//			if (mapping == -1)
	//				continue;

	//			const Tile tile = mMap.tilesets[0]->getTile(mapping);

	//			Vector2D<int> tileDimentions = tile.getRect().Size() * mapScale;
	//			Vector2D<int> tilePosition = Vector2D<int>(x, y) * tileDimentions;
	//			Rect<int> tileRect = Rect<int>(tilePosition, tileDimentions);

	//			if (camera.inView(tileRect))
	//			{
	//				tileRect = camera.toCameraCoords(tileRect);
	//				tile.render(tileRect);
	//			}
	//		}

	//		// render player
	//		//if (y == yPlayerTile && i == 1)
	//		//	mPlayer.render();
	//	}
	//}

	/* TODO create some generic rendering list to render objects
	for (int priority = 10; priority > -1; priority--)
	{
		for (Renderable* object : renderObjects)
		{
			if (object->priority == priority)
				object->render();
		}
	}
	*/


	mPlayer.render();
	
	for (int i = 0; i < mEnemies->size(); i++)
	{
		Enemy* enemy = mEnemies->getEnemy(i);

		if (camera.inView(enemy->getRect()))
		{
			enemy->render();
		}
	}

	mUIManager->render();

	mGameData->cursor->render();
}

