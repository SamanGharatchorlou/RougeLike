#pragma once

#include "Map/MapTile.h"
#include "Map/MapBase.h"

class Map;

class AIPathMap : public MapBase<PathTile>
{
public:
	void build(Map* map, int xSplit, int ySplit);
	void clear();

	const Index index(VectorF position) const;
	const Index index(const PathTile* tile) const;

	const VectorF tileSize() const;

	bool floorCollisionTile(Index index) const { return mData.get(index).collisionType() == MapTile::Floor; }

	const PathTile* offsetTile(const PathTile* target, int xOffset, int yOffset) const;

	void clearOccupiedTiles();
	void addOccupiedTile(VectorF position);

	void clearToBeOccupiedTiles();
	void addToBeOccupiedTile(Index index);

private:
	bool isValidPosition(VectorF position) const;

	std::vector<Index> occupiedTiles;
	std::vector<Index> toBeOccupiedTiles;
};