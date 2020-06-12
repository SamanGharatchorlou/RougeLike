#include "pch.h"
#include "Graphics/RenderManager.h"

#include "Map/Environment.h"
#include "UI/UIManager.h"
#include "Objects/Actors/ActorManager.h"
#include "Items/Collectables/Collectables.h"


RenderManager::RenderManager() 
	: mEnvironment(nullptr), mActors(nullptr), mUIManager(nullptr), mCollectables(nullptr) { }


void RenderManager::render()
{
	mEnvironment->renderBottomLayer();

	mActors->render();

	mEnvironment->renderTopLayer();

	mCollectables->render();
	
	mUIManager->render();
}
