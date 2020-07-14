#pragma once

#include "TrapManager.h"
#include "Map/MapBase.h"
#include "MapTile.h"

class TextureManager;
class Texture;


class Map : public MapBase<MapTile>
{
public:
	Map(Vector2D<int> mapIndexSize, VectorF tileSize);

	// Update
	void slowUpdate(float dt);

	// Rendering
	void renderFloor();
	void renderLowerLayer();
	void renderUpperLayer();

	void populateData(TextureManager* tm, VectorF offset);
	void clearData();

	void close(TextureManager* tm);

	// Getters
	Vector2D<float> size() const { return VectorF(xCount(), yCount()) * tileSize(); };
	const VectorF tileSize() const { return mTileSize; }

	const RectF getFirstRect() const;
	const RectF getLastRect() const;

	MapTile* tile(Index index) { return &mData[index]; };
	const MapTile* tile(Index index) const { return &mData.get(index); };
	const MapTile* tile(VectorF position) const;

	const Vector2D<int> index(VectorF position) const;
	const Vector2D<int> index(RectF rect) const;
	const Vector2D<int> index(const MapTile* tile) const;

	Vector2D<int> yTileFloorRange(VectorF position) const;

	bool isValidTile(RectF rect) const;
	bool isValidPosition(VectorF position) const;

	TrapManager& traps() { return mTrapManager; }


private:
	void populateTileRects(VectorF offset);


private:
	TrapManager mTrapManager;
	VectorF mTileSize;
};