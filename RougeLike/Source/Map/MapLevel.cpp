#include "pch.h"
#include "MapLevel.h"

#include "Game/GameData.h"
#include "TunnelGenerator.h"
#include "Map.h"
#include "Characters/Player/PlayerManager.h"


MapLevel::MapLevel(GameData* gameData) : mGameData(gameData)
{
	mMap = new Map();
	mMap->setScale(3.0f);

	mExitTunnel = new Map();
	mExitTunnel->setScale(3.0f);
};


void MapLevel::GenerateRandomLevel(int width, int height)
{
	mMap->init(width, height);

	TunnelGenerator generator;
	generator.buildRandom(mMap->getData());

	mMap->populateData();


	// Build tunnel
	mExitTunnel->init(20, height);
	generator.buildTunnel(mExitTunnel->getData());

	mExitTunnel->populateTileRects(VectorF(mMap->size().x, 0.0f));
	mExitTunnel->populateCollisionRenderInfo();
}


void MapLevel::renderA()
{
	float renderDepth = mGameData->playerManager->getRectRef()->Center().y;
	mMap->renderLayerA(mGameData->textureManager, renderDepth);
}


void MapLevel::renderB()
{
	float renderDepth = mGameData->playerManager->getRectRef()->Center().y;
	mMap->renderLayerB(mGameData->textureManager, renderDepth);
}


void MapLevel::renderExit()
{
	mExitTunnel->renderLayerA(mGameData->textureManager, mMap->size().y);
}


VectorF MapLevel::size() const 
{ 
	return mMap->size(); 
}


std::vector<Map*> MapLevel::maps() const
{
	return std::vector<Map*> { mMap, mExitTunnel };
}

Map* MapLevel::map(VectorF position) const
{
	if (mMap->getTile(position) != nullptr)
	{
		return mMap;
	}
	else
		printf("pause\n");
	
	
	if(mExitTunnel->getTile(position) != nullptr)
	{
		return mExitTunnel;
	}
	else
	{
		DebugPrint(Warning, "No map found at position: %f, %f\n", position.x, position.y);
	}
}