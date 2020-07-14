#pragma once

#include "Map/MapTile.h"
#include "Map/MapBase.h"

class Map;

class AIPathMap : public MapBase<PathTile>
{
public:
	void build(const Map* map, int xSplit, int ySplit);
	void clear();

	const Index index(VectorF position) const;
	const Index index(const PathTile* tile) const;

	//MapTile* tile(Index index) { return &mData[index]; };
	const PathTile* tile(Index index) const { return &mData.get(index); };
	//const MapTile* tile(VectorF position) const;

	const VectorF tileSize() const;

	bool floorCollisionTile(Index index) const { return mData.get(index).collisionType() == CollisionTile::Floor; }

	const PathTile* offsetTile(const PathTile* target, int xOffset, int yOffset) const;

	Grid<int>& costMap() { return mCostMap; }

private:
	bool isValidPosition(VectorF position) const;

	Grid<int> mCostMap;

	std::vector<Index> occupiedTiles;
	std::vector<Index> toBeOccupiedTiles;
};