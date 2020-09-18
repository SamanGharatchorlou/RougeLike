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

	// Warning: No bounds checking
	const Index index_fast(VectorF position) const;
	const Index index_fast(const PathTile* tile) const;

	const PathTile* tile(Index index) const { return &mData.get(index); };
	const PathTile* tile(VectorF position) const { return &mData.get(index(position)); };

	const VectorF tileSize() const;

	bool floorCollisionTile(Index index) const { return mData.get(index).collisionType() == CollisionTile::Floor; }

	const PathTile* offsetTile(const PathTile* target, int xOffset, int yOffset) const;

	const CostMap& costMap() const { return mCostMap; }
	CostMap& costMapRef() { return mCostMap; }

	Vector2D<int> yTileFloorRange(Index index) const;

	const PathTile* randomFloorTile(int xPointPercentage) const;
	const PathTile* randomFloorTile() const;

	bool isValidPosition(VectorF position) const;


private:
	CostMap mCostMap;
};