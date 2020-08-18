#pragma once

#include "MapBuilding/MapBuilder.h"
#include "TileInteractablesManager.h"

class Map;
class Camera;
class CollisionManager;
class EffectPool;

class LevelManager
{
public:
	LevelManager() : mLevel(0) { }

	void init(CollisionManager* collisions, EffectPool* effects);
	void load(const XMLParser& parser);
	void clear();

	void slowUpdate(float dt);
	void renderLowDepth();
	void renderHighDepth();

	int level() const { return mLevel; }
	void incrementLevel() { mLevel++; }

	Map* map(MapType type) const;
	Map* map(VectorF position) const;

	const Map* first() const { return mMaps.front(); }
	const Map* last() const { return mMaps.back(); }

	int mapCount() const { return mMaps.size(); }
	void addNextMap();
	void addMap(MapType type);
	void popFront();


private:
	VectorF getOffset(const Map* map) const;
	MapType nextMapType(MapType type);

	void setCameraBoundaries(Camera* camera);

	void closeLevel();

private:
	MapBuilder mBuilder;
	UniqueQueue<Map*> mMaps;
	TileInteractableManager mTrapManager;

	int mLevel;
};
