#include "pch.h"
#include "LevelManager.h"

#include "Tiles/TileTypes.h"
#include "Map.h"
#include "Game/Camera/Camera.h"


void LevelManager::init(Environment* environment)
{
	mLevel = 0;
	mBuilder.init(environment);
}



void LevelManager::load()
{
	mBuilder.load();

	Map* map = mBuilder.buildFirst();
	mMaps.push(map);

	addNextMap();
}


void LevelManager::clear()
{
	while (mMaps.size() > 0)
	{
		delete mMaps.pop();
	}

	mBuilder.clear();
	mLevel = 0;
}


// TODO: this causes the game to stutter (only in debug tbf)
// split the work up across a few frames?
void LevelManager::addNextMap()
{
	ASSERT(Error, mMaps.size() > 0, "Must have atleast one map in the map list to add the next one\n");

	VectorF offset = getOffset(mMaps.back());
	MapType type = nextMapType(mMaps.back()->type());

	Map* map = mBuilder.buildMap(type, offset);
	mMaps.push(map);

	if (map->type() == MapType::Dungeon)
	{
		LevelUpdatedEvent event(LevelUpdatedEvent::Added);
		notify(event);
	}
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

	return mMaps.front();
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

	return mMaps.back();
}


void LevelManager::slowUpdate(float dt)
{
	UniqueQueue<Map*>::iterator iter;
	for (iter = mMaps.begin(); iter != mMaps.end(); iter++)
	{

		Map* map = *iter;
		map->slowUpdate(dt);
	}

	Camera* camera = Camera::Get();
	if (lastMap(MapType::None)->midPoint().x < camera->rect().RightPoint())
	{
		addNextMap();
		setCameraBoundaries(camera);
	}

	closeLevel();
}


void LevelManager::pause()
{
	UniqueQueue<Map*>::iterator iter;
	for (iter = mMaps.begin(); iter != mMaps.end(); iter++)
	{
		(*iter)->pause();
	}

}
void LevelManager::resume()
{
	UniqueQueue<Map*>::iterator iter;
	for (iter = mMaps.begin(); iter != mMaps.end(); iter++)
	{
		(*iter)->resume();
	}
}

void LevelManager::closeLevel()
{
	Camera* camera = Camera::Get();

	if (mMaps.size() == 4)
	{
		if (camera->rect().LeftPoint() > lastMap(MapType::None)->getFirstRect().LeftPoint())
		{
			delete mMaps.pop();
			delete mMaps.pop();
			setCameraBoundaries(camera);
			
			LevelUpdatedEvent event(LevelUpdatedEvent::Removed);
			notify(event);
		}
	}

}


void LevelManager::renderFloor()
{
	UniqueQueue<Map*>::iterator iter;
	for (iter = mMaps.begin(); iter != mMaps.end(); iter++)
	{
		Map* map = *iter;
		map->renderFloor();
	}
}



void LevelManager::renderLowDepth()
{
	UniqueQueue<Map*>::iterator iter;
	for (iter = mMaps.begin(); iter != mMaps.end(); iter++)
	{
		Map* map = *iter;
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

	DebugPrint(Warning, "No map was found for position %f,%f\n", position.x, position.y);
	return nullptr;
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
	VectorF topLeft = firstMap(MapType::None)->getFirstRect().TopLeft();
	VectorF bottomRight = lastMap(MapType::None)->getBottomLastRect().BotRight();

	RectF boundaries(topLeft.x, topLeft.y, bottomRight.x, bottomRight.y);
	camera->setMapBoundaries(boundaries);
}