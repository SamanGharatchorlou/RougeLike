#include "pch.h"
#include "Graphics/RenderManager.h"

#include "Game/GameData.h"

#include "Game/Cursor.h"
#include "Map/Environment.h"
#include "UI/UIManager.h"

#include "Objects/Player/PlayerManager.h"
#include "Objects/Enemies/EnemyManager.h"
#include "Items/Collectables/Collectables.h"


RenderManager::RenderManager(GameData* gameData) :
	mGameData(gameData),
	mEnvironment(nullptr),
	mPlayer(nullptr),
	mEnemies(nullptr),
	mUIManager(nullptr) { }


void RenderManager::render()
{
	float renderDepth = mGameData->playerManager->rect().Center().y;

	mEnvironment->renderBottomLayer(mGameData->textureManager, renderDepth);

	mEnemies->render();

	mPlayer->render();

	mEnvironment->renderTopLayer(mGameData->textureManager, renderDepth);

	mCollectables->render();
	
	mUIManager->render();
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