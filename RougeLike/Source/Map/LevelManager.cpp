#include "pch.h"
#include "LevelManager.h"

#include "Tiles/TileTypes.h"
#include "Map.h"
#include "Game/Camera/Camera.h"




void LevelManager::addMap(MapType type)
{
	VectorF offset;
	if (mMaps.size() > 0)
		offset = getOffset(mMaps.back());

	Map* map = mBuilder.buildMap(type, offset);
	mMaps.push(map);
	mTrapManager.addMap(map, mBuilder.specs(map));
}


void LevelManager::addNextMap()
{
	VectorF offset = getOffset(mMaps.back());
	MapType type = nextMapType(mMaps.back()->type());

	Map* map = mBuilder.buildMap(type, offset);
	mMaps.push(map);
	mTrapManager.addMap(map, mBuilder.specs(map));
}


void LevelManager::popFront()
{
	mTrapManager.popFrontMap();
		
	Map* map = mMaps.popFront();
	mBuilder.returnMap(map);
}


void LevelManager::load(const XMLParser& parser)
{
	mTrapManager.load();

	Map* map = mBuilder.buildFirst();
	mMaps.push(map);
	mTrapManager.addMap(map, mBuilder.specs(map));

	addMap(MapType::Dungeon);
}


Map* LevelManager::map(MapType type) const
{
	UniqueQueue<Map*>::const_iterator iter;
	for (iter = mMaps.begin(); iter != mMaps.end(); iter++)
	{
		Map* map = *iter;

		if (type == map->type())
			return map;
	}

	return nullptr;
}


void LevelManager::slowUpdate(float dt)
{
	UniqueQueue<Map*>::iterator iter;
	for (iter = mMaps.begin(); iter != mMaps.end(); iter++)
	{
		Map* map = *iter;
		map->slowUpdate(dt);
	}

	mTrapManager.slowUpdate();

	Camera* camera = Camera::Get();
	if (last()->midPoint().x < camera->rect().RightPoint())
	{
		addNextMap();
		setCameraBoundaries(camera);
	}


	closeLevel();
}

void LevelManager::closeLevel()
{
	Camera* camera = Camera::Get();

	if (mMaps.size() == 4)
	{
		if (camera->rect().LeftPoint() > last()->getFirstRect().LeftPoint())
		{
			popFront();
			popFront();
			setCameraBoundaries(camera);
		}
	}

}


void LevelManager::renderLowDepth()
{
	UniqueQueue<Map*>::iterator iter;
	for (iter = mMaps.begin(); iter != mMaps.end(); iter++)
	{
		Map* map = *iter;
		map->renderFloor();
		map->renderLowerLayer();
		map->deferredRender();
	}
}


void LevelManager::renderHighDepth()
{
	UniqueQueue<Map*>::iterator iter;
	for (iter = mMaps.begin(); iter != mMaps.end(); iter++)
	{
		Map* map = *iter;
		map->renderUpperLayer();
	}
}


Map* LevelManager::map(VectorF position) const
{
	UniqueQueue<Map*>::const_iterator iter;
	for (iter = mMaps.begin(); iter != mMaps.end(); iter++)
	{
		Map* map = *iter;

		if (map->isValidPosition(position))
			return map;
	}
}


// --- Private Functions --- //
VectorF LevelManager::getOffset(const Map* map) const
{
	return VectorF(map->getLastRect().RightPoint(), 0.0f);
}


MapType LevelManager::nextMapType(MapType type)
{
	int incrementedType = (int)type + 1;
	MapType nextType = static_cast<MapType>(incrementedType);
	return (nextType == MapType::End) ? nextMapType(MapType::None) : nextType;
}

void LevelManager::setCameraBoundaries(Camera* camera)
{
	VectorF topLeft = first()->getFirstRect().TopLeft();
	VectorF bottomRight = last()->getBottomLastRect().BotRight();

	RectF boundaries(topLeft.x, topLeft.y, bottomRight.x, bottomRight.y);
	camera->setMapBoundaries(boundaries);
}