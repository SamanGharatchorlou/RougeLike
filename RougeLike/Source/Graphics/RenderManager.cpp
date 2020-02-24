#include "pch.h"
#include "Graphics/RenderManager.h"

#include "Game/GameData.h"

#include "Game/Cursor.h"
#include "Map/Map.h"
#include "UI/UIManager.h"
#include "Characters/Player/Player.h"
#include "Characters/Enemies/EnemyManager.h"


RenderManager::RenderManager(GameData* gameData) :
	mGameData(gameData),
	mMap(nullptr),
	mPlayer(nullptr),
	mEnemies(nullptr),
	mUIManager(nullptr) { }


void RenderManager::render()
{
	return;

	mMap->renderLayerA(mPlayer->getRect().Center().y);

	mEnemies->render();

	mPlayer->render();

	mMap->renderLayerB();
	
	mUIManager->render();

	mGameData->cursor->render();
}

