#pragma once

#include "Events/Dispatcher.h"
#include "MapBuilding/MapBuilder.h"


class Environment;
class Map;
class Camera;
class CollisionManager;
class EffectPool;
class Actor;


class LevelManager : public Dispatcher
{
public:
	LevelManager() : mLevel(0) { }

	void init(Environment* environment);
	void load();
	void clear();

	void slowUpdate(float dt);
	void pause();
	void resume();

	void renderFloor();
	void renderLowDepth();
	void renderHighDepth();

	int level() const { return mLevel; }
	void incrementLevel() { mLevel++; }

	// Type::None = any type
	Map* firstMap(MapType type) const;
	Map* lastMap(MapType type) const;
	Map* map(VectorF position) const;

	int mapCount() const { return mMaps.size(); }


private:
	void addNextMap();


private:
	VectorF getOffset(const Map* map) const;
	MapType nextMapType(MapType type);

	void setCameraBoundaries(Camera* camera);

	void closeLevel();

private:
	MapBuilder mBuilder;
	UniqueQueue<Map*> mMaps;

	int mLevel;
};
