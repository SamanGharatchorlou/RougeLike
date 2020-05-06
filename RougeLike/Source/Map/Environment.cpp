#include "pch.h"
#include "Environment.h"

#include "Game/Camera.h"
#include "TunnelGenerator.h"
#include "Map.h"


Environment::Environment() : mMapLevel(1)
{
	mPrimaryMap = new Map();
	mEntrace = new Map();
	mExit = nullptr;

	mMapSize = Vector2D<int>(30, 20);
};


void Environment::restart()
{
	delete mPrimaryMap;

	if(mEntrace)
		delete mEntrace;
	if(mExit)
		delete mExit;

	mPrimaryMap = new Map();
	mEntrace = new Map();
	mExit = nullptr;
}


void Environment::init()
{
	// create first entrace and level
	buildEntrance(0.0f);
	buildLevel();

	IncrementLevelEvent event;
	notify(Event::IncrementMapLevel, event);
}


void Environment::nextLevel()
{
	swapToEntrance();
	buildLevel();

	closeLevel();

	mMapLevel++;
	IncrementLevelEvent event;
	notify(Event::IncrementMapLevel, event);
}


void Environment::closeLevelEntrace()
{
	swapToExit();
	buildExit();

	closeEntrance();
}


void Environment::renderBottomLayer(const TextureManager* tm, float depth)
{
	Map* connectingTunnel = mExit ? mExit : mEntrace;
	connectingTunnel->renderBottomLayer(tm, depth);

	mPrimaryMap->renderBottomLayer(tm, depth);
}


void Environment::renderTopLayer(const TextureManager* tm, float depth)
{
	Map* connectingTunnel = mExit ? mExit : mEntrace;
	connectingTunnel->renderTopLayer(tm, depth);

	mPrimaryMap->renderTopLayer(tm, depth);
}


VectorF Environment::size() const
{
	return mPrimaryMap->size();
}


Map* Environment::map(VectorF position) const
{
	if (mEntrace && position.x < mEntrace->getLastRect().RightPoint())
		return mEntrace;

	else if (mPrimaryMap && position.x < mPrimaryMap->getLastRect().LeftPoint())
		return mPrimaryMap;

	else
		return mExit;
}


bool Environment::generateNextLevel(VectorF position) const
{
	if (position.x > mPrimaryMap->getLastRect().RightPoint())
	{
		RectF lastRect = mPrimaryMap->getLastRect(mPrimaryMap->yCount() / 2);

		// Make sure its completely out of view
		lastRect = lastRect.Translate(mPrimaryMap->tileSize());

		return !Camera::Get()->inView(lastRect);
	}
	else
		return false;
}


bool Environment::closeEntrance(VectorF position) const
{
	if (mEntrace && position.x > mEntrace->getLastRect().RightPoint())
	{
		RectF lastRect = mEntrace->getLastRect(mPrimaryMap->yCount() / 2);

		// Make sure its completely out of view
		lastRect = lastRect.Translate(mPrimaryMap->tileSize());

		return !Camera::Get()->inView(lastRect);
	}
	else
		return false;
}


RectF Environment::boundaries() const
{
	float xLeft = 0.0f;
	float xRight = 0.0f;

	if (mEntrace)
	{
		xLeft = mEntrace->getFirstRect().LeftPoint();
		xRight = mPrimaryMap->getLastRect().RightPoint();
	}
	else
	{
		xLeft = mPrimaryMap->getFirstRect().LeftPoint();
		xRight = mExit->getLastRect().RightPoint();
	}

	return RectF(xLeft, 0.0f, xRight, mPrimaryMap->size().y);
}


// --- Private Functions --- //

void Environment::buildEntrance(float offset)
{
	// Add a small buffer so the eixt tunnel takes up whole screen
	VectorF size(VectorF(16.0f, 16.0f) * 3.0f); // TODO: hard coded
	int width = (int)((Camera::Get()->size().x / size.x) * 1.5f);

	mEntrace->init(Index(width, mMapSize.y));

	TunnelGenerator generator;
	generator.buildSimpleLine(mEntrace->getData());

	mEntrace->populateData(VectorF(offset, 0.0f));
}


void Environment::buildLevel()
{
	ASSERT(Warning, mEntrace != nullptr, "Entrance cannot be null, did you forget to call swapToEntrace()?\n");

	// Random map width between 90% and 120% of previous level
	int mapMinX = (int)((float)mMapSize.x * 0.9f);
	int mapMaxX = (int)((float)mMapSize.x * 1.2f);

	int mapWidth = randomNumberBetween(mapMinX, mapMaxX);

	Index index(mEntrace->xCount() - 1, 0.0f);
	float offset = mEntrace->tileRect(index).RightPoint();

	buildRandomLevel(mapWidth, mMapSize.y, offset);
}


void Environment::buildRandomLevel(int width, int height, float offset)
{
	mMapSize = Vector2D<int>(width, height);

	mPrimaryMap->init(Index(width, height));

	TunnelGenerator generator;
	generator.buildRandom(mPrimaryMap->getData());

	mPrimaryMap->populateData(VectorF(offset, 0.0f));
}


void Environment::buildExit()
{
	ASSERT(Warning, mExit != nullptr, "Exit cannot be null, did you forget to call swapToExit()?\n");

	float offset = mPrimaryMap->getLastRect().RightPoint();

	// Add a small buffer so the eixt tunnel takes up whole screen
	int width = (int)((Camera::Get()->size().x / mPrimaryMap->tileSize().x) * 1.5f);

	mExit->init(Index(width, mMapSize.y));

	TunnelGenerator generator;
	generator.buildSimpleLine(mExit->getData());

	mExit->populateData(VectorF(offset, 0.0f));
}


void Environment::swapToEntrance()
{
	mEntrace = mExit;
	mExit = nullptr;
}


void Environment::swapToExit()
{
	mExit = mEntrace;
	mEntrace = nullptr;
}


void Environment::closeEntrance()
{
	mPrimaryMap->addTileType(Index(0, mPrimaryMap->yCount() / 2), MapTile::ColumnTop);
	mPrimaryMap->addTileType(Index(0, (mPrimaryMap->yCount() / 2) + 1), MapTile::ColumnBot);
}


void Environment::closeLevel()
{
	mEntrace->addTileType(Index(0, mEntrace->yCount() / 2), MapTile::ColumnTop);
	mEntrace->addTileType(Index(0, (mEntrace->yCount() / 2) + 1), MapTile::ColumnBot);

	// Open entrance
	mPrimaryMap->setTileType(Index(0, mPrimaryMap->yCount() / 2), MapTile::Floor);
	mPrimaryMap->setTileType(Index(0, (mPrimaryMap->yCount() / 2) + 1), MapTile::Floor);
}

