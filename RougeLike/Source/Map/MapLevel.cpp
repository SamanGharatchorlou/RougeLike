#include "pch.h"
#include "MapLevel.h"

#include "Game/Camera.h"
#include "TunnelGenerator.h"
#include "Map.h"


MapLevel::MapLevel()
{
	mMap = new Map();
	mEntrace = new Map();
	mExit = nullptr;

	mapSize = Vector2D<int>(30, 20);
};


void MapLevel::init()
{
	// create first entrace and level
	buildEntrance(0.0f);
	buildLevel();

	IncrementLevelEvent event;
	notify(Event::IncrementMapLevel, event);
}


void MapLevel::nextLevel()
{
	swapToEntrance();
	buildLevel();

	closeLevel();

	IncrementLevelEvent event;
	notify(Event::IncrementMapLevel, event);
}


void MapLevel::closeLevelEntrace()
{
	swapToExit();
	buildExit();

	closeEntrance();
}


void MapLevel::renderBottomLayer(const TextureManager* tm, float depth)
{
	Map* connectingTunnel = mExit ? mExit : mEntrace;
	connectingTunnel->renderBottomLayer(tm, depth);

	mMap->renderBottomLayer(tm, depth);
}


void MapLevel::renderTopLayer(const TextureManager* tm, float depth)
{
	Map* connectingTunnel = mExit ? mExit : mEntrace;
	connectingTunnel->renderTopLayer(tm, depth);

	mMap->renderTopLayer(tm, depth);
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


bool MapLevel::generateNextLevel(VectorF position) const
{
	if (position.x > mMap->getLastRect().RightPoint())
	{
		RectF lastRect = mMap->getLastRect(mMap->yCount() / 2);

		// Make sure its completely out of view
		lastRect = lastRect.Translate(mMap->tileSize());

		return !Camera::Get()->inView(lastRect);
	}
	else
		return false;
}


bool MapLevel::closeEntrance(VectorF position) const
{
	if (mEntrace && position.x > mEntrace->getLastRect().RightPoint())
	{
		RectF lastRect = mEntrace->getLastRect(mMap->yCount() / 2);

		// Make sure its completely out of view
		lastRect = lastRect.Translate(mMap->tileSize());

		return !Camera::Get()->inView(lastRect);
	}
	else
		return false;
}


RectF MapLevel::boundaries() const
{
	float xLeft = 0.0f;
	float xRight = 0.0f;

	if (mEntrace)
	{
		xLeft = mEntrace->getFirstRect().LeftPoint();
		xRight = mMap->getLastRect().RightPoint();
	}
	else
	{
		xLeft = mMap->getFirstRect().LeftPoint();
		xRight = mExit->getLastRect().RightPoint();
	}

	return RectF(xLeft, 0.0f, xRight, mMap->size().y);
}


// --- Private Functions --- //

void MapLevel::buildEntrance(float offset)
{
	// Add a small buffer so the eixt tunnel takes up whole screen
	VectorF size(VectorF(16.0f, 16.0f) * 3.0f); // TODO: hard coded
	int width = (int)((Camera::Get()->size().x / size.x) * 1.5f);

	mEntrace->init(width, mapSize.y);

	TunnelGenerator generator;
	generator.buildSimpleLine(mEntrace->getData());

	mEntrace->populateData(VectorF(offset, 0.0f));
}


void MapLevel::buildLevel()
{
	ASSERT(Warning, mEntrace != nullptr, "Entrance cannot be null, did you forget to call swapToEntrace()?\n");

	// Random map width between 90% and 120% of previous level
	int mapMinX = (int)((float)mapSize.x * 0.9f);
	int mapMaxX = (int)((float)mapSize.x * 1.2f);

	int mapWidth = randomNumberBetween(mapMinX, mapMaxX);

	float offset = mEntrace->tileRect(mEntrace->xCount() - 1, 0.0f).RightPoint();

	buildRandomLevel(mapWidth, mapSize.y, offset);
}


void MapLevel::buildRandomLevel(int width, int height, float offset)
{
	mapSize = Vector2D<int>(width, height);

	mMap->init(width, height);

	TunnelGenerator generator;
	generator.buildRandom(mMap->getData());

	mMap->populateData(VectorF(offset, 0.0f));
}


void MapLevel::buildExit()
{
	ASSERT(Warning, mExit != nullptr, "Exit cannot be null, did you forget to call swapToExit()?\n");

	float offset = mMap->getLastRect().RightPoint();

	// Add a small buffer so the eixt tunnel takes up whole screen
	int width = (int)((Camera::Get()->size().x / mMap->tileSize().x) * 1.5f);

	mExit->init(width, mapSize.y);

	TunnelGenerator generator;
	generator.buildSimpleLine(mExit->getData());

	mExit->populateData(VectorF(offset, 0.0f));
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


void MapLevel::closeEntrance()
{
	mMap->addTileType(0, mMap->yCount() / 2, MapTile::ColumnTop);
	mMap->addTileType(0, (mMap->yCount() / 2) + 1, MapTile::ColumnBot);
}


void MapLevel::closeLevel()
{
	mEntrace->addTileType(0, mEntrace->yCount() / 2, MapTile::ColumnTop);
	mEntrace->addTileType(0, (mEntrace->yCount() / 2) + 1, MapTile::ColumnBot);

	// Open entrance
	mMap->setTileType(0, mMap->yCount() / 2, MapTile::Floor);
	mMap->setTileType(0, (mMap->yCount() / 2) + 1, MapTile::Floor);
}

