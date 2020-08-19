#pragma once

#include "Map/Tiles/MapTile.h"
#include "Map/MapBase.h"

class Map;

class AIPathMap : public MapBase<PathTile>
{
public:
	void build(const Map* map, int xSplit, int ySplit);
	void clear();

	const Index index(VectorF position) const;
	const Index index(const PathTile* tile) const;

	const PathTile* tile(Index index) const { return &mData.get(index); };

	const VectorF tileSize() const;

	bool floorCollisionTile(Index index) const { return mData.get(index).collisionType() == CollisionTile::Floor; }

	const PathTile* offsetTile(const PathTile* target, int xOffset, int yOffset) const;

	const Grid<int>& costMap() const { return mCostMap; }
	Grid<int>& costMapRef() { return mCostMap; }


	bool isValidPosition(VectorF position) const;
private:


private:
	Grid<int> mCostMap;
};