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
	LevelManager(CollisionManager* collisions, EffectPool* effects) : mTrapManager(collisions, effects), mLevel(0) { }

	void load(const XMLParser& parser);

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
