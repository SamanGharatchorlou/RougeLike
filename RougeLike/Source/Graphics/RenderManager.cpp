#include "pch.h"
#include "Graphics/RenderManager.h"

#include "Game/GameData.h"

#include "Game/Cursor.h"
#include "Map/Map.h"
#include "UI/UIManager.h"
#include "Characters/Player/PlayerManager.h"
#include "Characters/Enemies/EnemyManager.h"
#include "Items/Collectables/Collectables.h"


RenderManager::RenderManager(GameData* gameData) :
	mGameData(gameData),
	mMap(nullptr),
	mPlayer(nullptr),
	mEnemies(nullptr),
	mUIManager(nullptr) { }


void RenderManager::render()
{
	//return;


	mMap->renderLayerA(mPlayer->getRectRef()->Center().y);

	mEnemies->render();

	mPlayer->render();

	mMap->renderLayerB();

	mCollectables->render();
	
	mUIManager->render();

	mGameData->cursor->render();
}

//void RenderManager::add(Renderable* renderable)
//{
//	renderables.push_back(renderable);
//}
//
//
//void RenderManager::render()
//{
//	for (unsigned int i = 0; i < renderables.size(); i++)
//	{
//		renderables[i]->render();
//	}
//}