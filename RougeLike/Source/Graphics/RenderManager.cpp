#include "pch.h"
#include "Graphics/RenderManager.h"

#include "Map/Environment.h"
#include "UI/UIManager.h"
#include "Objects/Actors/ActorManager.h"
#include "Items/Collectables/Collectables.h"

#include "Graphics/Texture.h"


RenderManager::RenderManager() 
	: mEnvironment(nullptr), mActors(nullptr), mUIManager(nullptr), mCollectables(nullptr) { }


void RenderManager::render()
{
	renderPackets(RenderLayer::Lowest);

	mEnvironment->renderFloor();

	renderPackets(RenderLayer::Floor);

	mEnvironment->renderBottomLayer();

	renderPackets(RenderLayer::LowerTiles);

	mActors->render();

	renderPackets(RenderLayer::Actors);

	mCollectables->render();

	renderPackets(RenderLayer::Items);

	mEnvironment->renderTopLayer();

	renderPackets(RenderLayer::UpperTiles);
	
	mUIManager->render();

	renderPackets(RenderLayer::UI);
	renderPackets(RenderLayer::Highest);

	mRenderPackets.clear();
}


void RenderManager::renderPackets(RenderLayer layer)
{
	for (int i = 0; i < mRenderPackets.size(); i++)
	{
		if(mRenderPackets[i].layer == layer)	
		{
			mRenderPackets[i].texture->render(mRenderPackets[i].rect);
		}
	}
}
