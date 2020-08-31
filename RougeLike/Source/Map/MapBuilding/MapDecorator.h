#pragma once

#include "Utilities/Maps/DecorMap.h"

class Map;
class MapTile;
enum class DecorType : Uint32;


class MapDecorator
{
public:
	void addDecor(Map* map, const DecorMap& decorMap);

private:

	void addColumns(Grid<MapTile>& data,		const DecorMap& decorMap);
	void addWaterFeatures(Grid<MapTile>& data,	const DecorMap& decorMap);
	void addTorches(Grid<MapTile>& data,		const DecorMap& decorMap);
	void addSpikes(Grid<MapTile>& data,			const DecorMap& decorMap);
	void addTiggers(Grid<MapTile>& data,		const DecorMap& decorMap);
	void addGrating(Grid<MapTile>& data,		const DecorMap& decorMap);

	bool doesTypeContain(DecorType baseType, DecorType queryType) const;


	bool canAddWater(const Grid<MapTile>& data, Index index, Vector2D<int> size) const;

	bool canAddWallDecor(const Grid<MapTile>& data, DecorType decor, Index lowerIndex, int spacing) const;
	bool isLowerWall(const Grid<MapTile>& data, Index index) const;
};

