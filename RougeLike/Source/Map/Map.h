#pragma once

#include "Map/MapBase.h"
#include "MapTile.h"

class TextureManager;
class Texture;


class Map : public MapBase<MapTile>
{
public:
	Map(Vector2D<int> ampIndexSize, VectorF tileSize);

	void populateData(TextureManager* tm, VectorF offset);
	void clearData();

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
	bool wallRenderTile(Index index) const { return mData.get(index).renderType() >= MapTile::Wall; }
	bool floorRenderTile(Index index) const { return mData.get(index).renderType() == MapTile::Floor; }

	bool wallCollisionTile(Index index) const { return mData.get(index).collisionType() >= MapTile::Wall; }
	bool floorCollisionTile(Index index) const { return mData.get(index).collisionType() == MapTile::Floor; }
	bool floorCollisionTile(VectorF position) const { return mData.get(index(position)).collisionType() == MapTile::Floor; }

	bool isValidTile(RectF rect) const;
	bool isValidPosition(VectorF position) const;

	// Rendering
	void renderFloor();
	void renderTop();
	void renderBottom();

private:
	void populateTileRects(VectorF offset);

	// Collision Info
	void populateTileCollisionInfo();

	// Render info
	void populateTileRenderInfo(TextureManager* tm);
	void topBottom();
	void leftRight();
	void corners();
	void pointCorners();
	void cleanLabels();
	void setTextures(TextureManager* tm);

#if _DEBUG
	void renderSurfaceTypes();
#endif

private:
	VectorF mTileSize;
};