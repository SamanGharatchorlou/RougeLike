#pragma once

#include "MapSpecifications.h"

class Map;
enum class MapType;


class MapBuilder
{
public:
	void load();
	void clear();

	Map* buildFirst();
	Map* buildMap(MapType type, VectorF offset);

	void returnMap(Map* map);

	void close(Map* map);

	const DecorMap& specs(Map* map) { return mSpecs.getDecor(map); }

private:
	void buildMapStructure(Map* map, MapType type);
	void populateMapRects(Map* map, VectorF offset);
	void populateMapData(Map* map);
	void addMapDecor(Map* map);
	void addTrapMechanics(Map* map);

private:
	UniqueQueue<Map*> mPool;

	MapSpecifications mSpecs;
};