#include "pch.h"
#include "LevelManager.h"

#include "Tiles/TileTypes.h"
#include "Map.h"
#include "Game/Camera/Camera.h"



void LevelManager::init(CollisionManager* collisions, EffectPool* effects)
{
	mLevel = 0;
	mTrapManager.init(collisions, effects);
}



void LevelManager::load(const XMLParser& parser)
{
	mBuilder.load();

	mTrapManager.load();

	Map* map = mBuilder.buildFirst();
	mMaps.push(map);
	mTrapManager.addMap(map, mBuilder.specs(map));

	addMap(MapType::Dungeon);
}


void LevelManager::clear()
{
	while (mMaps.size() > 0)
	{
		mBuilder.returnMap(mMaps.popFront());
	}

	mBuilder.clear();
	mTrapManager.clear();
	mLevel = 0;
}



void LevelManager::addMap(MapType type)
{
	VectorF offset;
	if (mMaps.size() > 0)
		offset = getOffset(mMaps.back());

	Map* map = mBuilder.buildMap(type, offset);
	mMaps.push(map);
	mTrapManager.addMap(map, mBuilder.specs(map));

	if (map->type() == MapType::Dungeon)
	{
		LevelUpdatedEvent event(LevelUpdatedEvent::Added);
		notify(event);
	}
}


void LevelManager::addNextMap()
{
	VectorF offset = getOffset(mMaps.back());
	MapType type = nextMapType(mMaps.back()->type());

	Map* map = mBuilder.buildMap(type, offset);
	mMaps.push(map);
	mTrapManager.addMap(map, mBuilder.specs(map));

	if (map->type() == MapType::Dungeon)
	{
		LevelUpdatedEvent event(LevelUpdatedEvent::Added);
		notify(event);
	}
}


void LevelManager::popFront()
{
	mTrapManager.popFrontMap();
		
	Map* map = mMaps.popFront();
	mBuilder.returnMap(map);
}



Map* LevelManager::firstMap(MapType type) const
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


Map* LevelManager::lastMap(MapType type) const
{
	UniqueQueue<Map*>::const_rIterator rIter;
	for (rIter = mMaps.rbegin(); rIter != mMaps.rend(); rIter++)
	{
		Map* map = *rIter;

		if (type == map->type())
			return map;
	}

	return nullptr;
}


int LevelManager::mapCount(MapType type) const
{
	int count = 0;
	UniqueQueue<Map*>::const_iterator iter;
	for (iter = mMaps.begin(); iter != mMaps.end(); iter++)
	{
		Map* map = *iter;

		if (type == map->type())
			count++;
	}

	return count;
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
			
			LevelUpdatedEvent event(LevelUpdatedEvent::Removed);
			notify(event);
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