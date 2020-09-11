#include "pch.h"
#include"Map/Map.h"

#include "Game/Camera/Camera.h"

#if DEBUG_CHECK
#include "Debug/MapDebugging.h"
#include "Debug/DebugDraw.h"
#endif

Map::Map() : mTraps(this), mType(MapType::None) { }

Map::Map(Vector2D<int> mapIndexSize) : MapBase(mapIndexSize), mType(MapType::None), mTraps(this) { }


void Map::setSize(Vector2D<int> size)
{
	mData.set(size, MapTile());
}

void Map::buildDeferredRenderList()
{
	for (int y = 0; y < yCount(); y++)
	{
		for (int x = 0; x < xCount(); x++)
		{
			Index index(x, y);
			MapTile* tile = &mData[index];

			if (tile->deferRender())
				mDeferredRendering.push_back(tile);
		}
	}
}


void Map::slowUpdate(float dt)
{
	for (int y = 0; y < yCount(); y++)
	{
		for (int x = 0; x < xCount(); x++)
		{
			Index index(x, y);
			MapTile* tile = &mData[index];

			tile->slowUpdate(dt);
		}
	}

	mTraps.slowUpdate();
}


void Map::renderLowerLayer()
{
	Camera* camera = Camera::Get();
	for (unsigned int x = 0; x < xCount(); x++)
	{
		for (unsigned int y = 0; y < yCount(); y++)
		{
			Index index(x, y);
			MapTile tile = mData[index];

			if(tile.is(CollisionTile::Floor))
				break;

			render(&tile, camera);
		}
	}
}


void Map::renderUpperLayer()
{
	Camera* camera = Camera::Get();

	for (unsigned int x = 0; x < xCount(); x++)
	{
		for (unsigned int y = yCount() - 1; y > 0; y--)
		{
			Index index(x, y);
			MapTile tile = mData[index];

			if (tile.renderType() < RenderTile::Wall)
				break;

			render(&tile, camera);
		}
	}

#if RENDER_SURFACE_TYPES
	renderSurfaceTypes(mData);
#endif
	int a = 4;
#if MAP_BOUNDARIES
	renderMapBoundaries(this);
#endif
}

void Map::deferredRender()
{
	Camera* camera = Camera::Get();

	for (int i = 0; i < mDeferredRendering.size(); i++)
	{
		MapTile* tile = mDeferredRendering[i];	
		RectF tileRect = tile->rect();
		if (camera->inView(tileRect))
		{
			tileRect = camera->toCameraCoords(tileRect);
			tile->deferredRender(tileRect);
		}
	}
}


// --- Getters --- //
VectorF Map::tileSize() const
{
	return getFirstRect().Size();
}

Vector2D<float> Map::size() const 
{ 
	VectorF tileCount = VectorF((float)xCount(), (float)yCount());
	return tileCount * tileSize();
};

const Index Map::index(VectorF position) const
{
	VectorF mapTopLeft = mData.get(Index(0, 0)).rect().TopLeft();
	VectorF shiftedPosition = position - mapTopLeft;
	return isValidPosition(position) ? (shiftedPosition / tileSize()).toInt() : Index(-1, -1);
}


const Index Map::index(const MapTile* tile) const
{
	VectorF position(tile->rect().TopLeft());
	return index(position);
}


const Index Map::index(RectF rect) const
{               
	VectorF position(rect.TopLeft());
	return index(position);
}


const MapTile* Map::tile(VectorF position) const 
{
	Index tileIndex = index(position);
	return isValidIndex(tileIndex) ? &mData.get(tileIndex) : nullptr;
}


const RectF Map::getFirstRect() const
{
	return mData.get(Index(0, 0)).rect();
}


const RectF Map::getLastRect() const
{
	return  mData.get(Index(xCount() - 1, 0)).rect();
}

const RectF Map::getBottomLastRect() const
{
	return  mData.get(Index(xCount() - 1, yCount() - 1)).rect();
}

VectorF Map::midPoint() const
{
	int xMid = xCount() / 2;
	int yMid = yCount() / 2;
	return tile(Index(xMid, yMid))->rect().Center();
}


const MapTile* Map::randomFloorTile(int xPointPercentage) const
{
	int xTileIndex = (int)((xCount() * xPointPercentage) / 100);

	std::vector<Index> floorIndexes;
	for (int y = 0; y < yCount(); y++)
	{
		Index index(xTileIndex, y);
		if (tile(index)->is(CollisionTile::Floor))
			floorIndexes.push_back(index);
	}

	// TODO: -1 -> we dont want to include the 'hidden' floor under the parralax walls?
	int randomIndex = randomNumberBetween(0, floorIndexes.size() - 1);
	Index index(floorIndexes[randomIndex]);
	return tile(index);
}


const MapTile* Map::randomFloorTile() const
{
	int randomX = randomNumberBetween(0, 101);
	return randomFloorTile(randomX);
}


// -- Validity functions -- //
bool Map::isValidTile(RectF rect) const
{
	VectorF start = mData.get(Index(0, 0)).rect().TopLeft();
	VectorF end = mData.get(Index(xCount(), yCount()) - 1).rect().BotRight();

	return (rect.x1 >= start.x && rect.y1 >= start.y) &&
			(rect.x2 < end.x && rect.y2 < end.y) &&
			 rect.Size() == tileSize();
}


bool Map::isValidPosition(VectorF position) const
{
	VectorF start = mData.get(Index(0, 0)).rect().TopLeft();
	VectorF end = mData.get(Index(xCount(), yCount()) - 1).rect().BotRight();

	return (position.x >= start.x && position.x < end.x) &&
			(position.y >= start.y && position.y < end.y);
}


// --- Private Function --- //
void Map::renderFloor()
{
	Camera* camera = Camera::Get();

	for (unsigned int y = 0; y < yCount(); y++)
	{
		for (unsigned int x = 0; x < xCount(); x++)
		{
			Index index(x, y);
			MapTile tile = mData[index];

			if (tile.has(CollisionTile::Floor | CollisionTile::Water))
			{
				render(&tile, camera);
			}
		}
	}
}

void Map::render(MapTile* tile, Camera* camera)
{
	RectF tileRect = tile->rect();
	if (camera->inView(tileRect))
	{
		tileRect = camera->toCameraCoords(tileRect);
		tile->render(tileRect);
	}
}



void Map::initTrapManager(Actor* actor, EffectPool* effectPool, const TrapDataMap* trapData)
{
	mTraps.init(actor, effectPool, trapData);
}