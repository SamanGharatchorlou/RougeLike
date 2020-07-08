#pragma once

#include "Map/MapBase.h"
#include "MapTile.h"

class TextureManager;
class Texture;


class Map : public MapBase<MapTile>
{
public:
	Map(Vector2D<int> mapIndexSize, VectorF tileSize);

	void populateData(TextureManager* tm, VectorF offset);
	void clearData();

	void close(TextureManager* tm);

	// Getters
	Vector2D<float> size() const { return VectorF(xCount(), yCount()) * tileSize(); };
	const VectorF tileSize() const { return mTileSize; }

	const RectF getFirstRect() const;
	const RectF getLastRect() const;

	const MapTile* tile(Index index) const { return MapBase<MapTile>::tile(index); };
	const MapTile* tile(VectorF position) const;

	const Vector2D<int> index(VectorF position) const;
	const Vector2D<int> index(RectF rect) const;
	const Vector2D<int> index(const MapTile* tile) const;

	// Query tiles
	CollisionTile collisionType(Index index) const { return mData.get(index).collisionType(); }

	bool isValidTile(RectF rect) const;
	bool isValidPosition(VectorF position) const;

	// Rendering
	void renderFloor();
	virtual void renderLowerLayer();
	void renderUpperLayer();


private:
	void populateTileRects(VectorF offset);


#if _DEBUG
	void renderSurfaceTypes();
#endif

private:
	VectorF mTileSize;
};