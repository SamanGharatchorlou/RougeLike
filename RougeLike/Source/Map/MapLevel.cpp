#include "pch.h"
#include "MapLevel.h"

#include "Game/GameData.h"
#include "Game/Camera.h"
#include "TunnelGenerator.h"
#include "Map.h"
#include "Characters/Player/PlayerManager.h"


MapLevel::MapLevel()
{
	mMap = new Map();
	mMap->setScale(3.0f);

	mEntrace = new Map();
	mEntrace->setScale(3.0f);

	mExit = nullptr;

	mapSize = Vector2D<int>(30, 20);
};



void MapLevel::generateNextLevel()
{
	ASSERT(Warning, mEntrace != nullptr, "Entrance cannot be null, did you forget to call swapToEntrace()?\n");

	// Random map width between 90% and 120% of previous level
	int mapMinX = (int)((float)mapSize.x * 0.9f);
	int mapMaxX = (int)((float)mapSize.x * 1.2f);

	int mapWidth = randomNumberBetween(mapMinX, mapMaxX);

	float offset = mEntrace->getTileRect(mEntrace->xCount() - 1, 0.0f).RightPoint();

	generateRandomLevel(mapWidth, mapSize.y, offset);
}




void MapLevel::generateRandomLevel(int width, int height, float offset)
{
	mapSize = Vector2D<int>(width, height);

	mMap->init(width, height);

	TunnelGenerator generator;
	generator.buildRandom(mMap->getData());

	mMap->populateData(VectorF(offset, 0.0f));
}


void MapLevel::generateEntrace(float offset)
{
	// Add a small buffer so the eixt tunnel takes up whole screen
	int width = (int)((Camera::Get()->size().x / mMap->getTileSize().x) * 1.5f);

	mEntrace->init(width, mapSize.y);

	TunnelGenerator generator;
	generator.buildSimpleLine(mEntrace->getData());

	mEntrace->populateData(VectorF(offset, 0.0f));
}

void MapLevel::generateExit(float offset)
{
	// Add a small buffer so the eixt tunnel takes up whole screen
	int width = (int)((Camera::Get()->size().x / mMap->getTileSize().x) * 1.5f);

	mExit->init(width, mapSize.y);

	TunnelGenerator generator;
	generator.buildSimpleLine(mExit->getData());

	mExit->populateData(VectorF(offset, 0.0f));
}


void MapLevel::generateNextExit()
{
	ASSERT(Warning, mExit != nullptr, "Exit cannot be null, did you forget to call swapToExit()?\n");

	float offset = mMap->getLastRect().RightPoint();
	generateExit(offset);
}


void MapLevel::renderA(const TextureManager* tm, float depth)
{
	mMap->renderLayerA(tm, depth);

	Map* connectingTunnel = mExit ? mExit : mEntrace;
	connectingTunnel->renderLayerA(tm, depth);
}


void MapLevel::renderB(const TextureManager* tm, float depth)
{
	mMap->renderLayerB(tm, depth);

	Map* connectingTunnel = mExit ? mExit : mEntrace;
	connectingTunnel->renderLayerB(tm, depth);
}


VectorF MapLevel::size() const 
{ 
	return mMap->size(); 
}


Map* MapLevel::map(VectorF position) const
{
	if (mEntrace && position.x < mEntrace->getLastRect().RightPoint())
		return mEntrace;

	else if (mMap && position.x < mMap->getLastRect().LeftPoint())
		return mMap;
	
	else
		return mExit;
}


bool MapLevel::mapOutOfView(VectorF position) const
{
	if (position.x > mMap->getLastRect().RightPoint())
		return !Camera::Get()->inView(mMap->getLastRect(mMap->yCount() / 2));
	else
		return false;
}


bool MapLevel::entraceOutOfView(VectorF position) const
{
	if (mEntrace)
	{
		if (position.x > mEntrace->getLastRect().RightPoint())
			return !Camera::Get()->inView(mEntrace->getLastRect(mMap->yCount() / 2));
	}
	else
		DebugPrint(Warning, "Entrance has not been set\n");

	return false;
}


void MapLevel::swapToEntrance()
{
	mEntrace = mExit;
	mExit = nullptr;
}


void MapLevel::swapToExit()
{
	mExit = mEntrace;
	mEntrace = nullptr;
}