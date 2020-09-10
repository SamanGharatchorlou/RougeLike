#pragma once

#include "MapSpecifications.h"

class Environment;
class EffectPool;
class Actor;
class Map;
enum class MapType;


class MapBuilder
{
public:
	MapBuilder();
	void init(Environment* environment);

	void load();
	void clear();

	Map* buildFirst();
	Map* buildMap(MapType type, VectorF offset);

	//void returnMap(Map* map);

	void close(Map* map);

	const DecorMap& specs(Map* map) { return mSpecs.getDecor(map); }

private:
	void fillMapData(Map* map, MapType type, VectorF offset, int level);

	void buildMapStructure(Map* map, MapType type);
	void populateMapRects(Map* map, VectorF offset);
	void populateMapData(Map* map);
	void addMapDecor(Map* map);

	void setMapLevelSpikeRate(DecorMap& decorMap, int mapLevel);

private:
	EffectPool* mEffectPool;
	Actor* mPlayer;

	MapSpecifications mSpecs;
	int mMapLevel;
};