#include "pch.h"
#include "Graphics/RenderManager.h"

#include "Game/GameData.h"

#include "Game/Cursor.h"
#include "Map/Environment.h"
#include "UI/UIManager.h"

#include "Objects/Actors/ActorManager.h"
#include "Objects/Actors/Player/Player.h"
#include "Items/Collectables/Collectables.h"


RenderManager::RenderManager(GameData* gameData) :
	mGameData(gameData),
	mEnvironment(nullptr),
	mActors(nullptr),
	mUIManager(nullptr) { }


void RenderManager::render()
{
	mEnvironment->renderBottomLayer();

	mActors->render();

	mEnvironment->renderTopLayer();

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