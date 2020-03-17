#include "pch.h"
#include "MapLevel.h"

#include "Game/GameData.h"
#include "Game/Camera.h"
#include "TunnelGenerator.h"
#include "Map.h"
#include "Characters/Player/PlayerManager.h"


MapLevel::MapLevel(GameData* gameData) : mGameData(gameData), mEntraceTunnel(nullptr)
{
	mMap = new Map();
	mMap->setScale(3.0f);

	mExitTunnel = new Map();
	mExitTunnel->setScale(3.0f);

	mapSize = Vector2D<int>(30, 20);
};



void MapLevel::generateNextLevel()
{
	swapToEntrance();

	// Random map width between 90% and 120% of previous level
	int mapMinX = (int)((float)mapSize.x * 0.9f);
	int mapMaxX = (int)((float)mapSize.x * 1.2f);

	int mapWidth = randomNumberBetween(mapMinX, mapMaxX);

	float offset = mEntraceTunnel->getTileRect(mEntraceTunnel->xCount() - 1, 0.0f).RightPoint();

	generateRandomLevel(mapWidth, mapSize.y, offset);
}




void MapLevel::generateRandomLevel(int width, int height, float offset)
{
	mapSize = Vector2D<int>(width, height);

	mMap->init(width, height);

	TunnelGenerator generator;
	generator.buildRandom(mMap->getData(), width);

	mMap->populateData(VectorF(offset, 0.0f));
}


void MapLevel::generateExitTunnel()
{
	swapToExit();

	// Add a small buffer so the eixt tunnel takes up whole screen
	int exitTunnelWidth = (int)((Camera::Get()->size().x / mMap->getTileSize().x) * 1.5f);

	mExitTunnel->init(exitTunnelWidth, mapSize.y);

	TunnelGenerator generator;
	generator.buildExit(mExitTunnel->getData(), exitTunnelWidth);

	mExitTunnel->populateData(VectorF(mMap->size().x, 0.0f));
}





void MapLevel::renderA()
{
	float renderDepth = mGameData->playerManager->getRectRef()->Center().y;
	mMap->renderLayerA(mGameData->textureManager, renderDepth);

	Map* connectingTunnel = mExitTunnel ? mExitTunnel : mEntraceTunnel;
	connectingTunnel->renderLayerA(mGameData->textureManager, renderDepth);
}


void MapLevel::renderB()
{
	float renderDepth = mGameData->playerManager->getRectRef()->Center().y;
	mMap->renderLayerB(mGameData->textureManager, renderDepth);

	Map* connectingTunnel = mExitTunnel ? mExitTunnel : mEntraceTunnel;
	connectingTunnel->renderLayerB(mGameData->textureManager, renderDepth);
}


VectorF MapLevel::size() const 
{ 
	return mMap->size(); 
}


Map* MapLevel::map(VectorF position) const
{
	if (mEntraceTunnel && position.x < mEntraceTunnel->getLastRect().RightPoint())
		return mEntraceTunnel;

	else if (mMap && position.x < mMap->getLastRect().LeftPoint())
		return mMap;
	
	else
		return mExitTunnel;
}


bool MapLevel::mapOutOfView() const
{
	const RectF playerRect = *mGameData->playerManager->getRectRef();

	if (playerRect.LeftPoint() > mMap->getLastRect().RightPoint())
		return !Camera::Get()->inView(mMap->getLastRect(mMap->yCount() / 2));
	else
		return false;
}


bool MapLevel::entraceOutOfView() const
{
	if (mEntraceTunnel)
	{
		const RectF playerRect = *mGameData->playerManager->getRectRef();

		if (playerRect.LeftPoint() > mEntraceTunnel->getLastRect().RightPoint())
			return !Camera::Get()->inView(mEntraceTunnel->getLastRect(mMap->yCount() / 2));
		else
			return false;

	}
	else
		DebugPrint(Warning, "Entrance has not been set\n");

	return true;
}


void MapLevel::swapToEntrance()
{
	mEntraceTunnel = mExitTunnel;
	mExitTunnel = nullptr;
}


void MapLevel::swapToExit()
{
	mExitTunnel = mEntraceTunnel;
	mEntraceTunnel = nullptr;
}