#include "pch.h"
#include "Graphics/RenderManager.h"

#include "Map/Environment.h"
#include "UI/UIManager.h"
#include "Graphics/Texture.h"


RenderManager::RenderManager() 
	: mEnvironment(nullptr), mUIManager(nullptr) { }


void RenderManager::render()
{
	renderPackets(RenderLayer::Lowest);

	mEnvironment->renderBottomLayer();

	renderPackets(RenderLayer::LowerTiles);

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
