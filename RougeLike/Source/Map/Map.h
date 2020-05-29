#pragma once

#include "Map/MapBase.h"
#include "MapTile.h"

class TextureManager;
class Texture;


class Map : public MapBase<MapTile>
{
public:
	void init(Vector2D<int> size);
	void populateData(VectorF offset = VectorF());


	// TEMP
	void populateTileRenderInfo();
	void render(const TextureManager* tm);
#if MARK_SURFACE_TYPES
	void renderSurfaceTypes();
#endif


	Vector2D<float> size() const { return VectorF(xCount(), yCount()) * tileSize(); };
	const VectorF tileSize() const { return tile(Index(0, 0))->rect().Size(); }

	const RectF getFirstRect() const;
	const RectF getLastRect() const;

	const MapTile* tile(Index index) const { return MapBase<MapTile>::tile(index); };
	const MapTile* tile(VectorF position) const;

	const Vector2D<int> index(VectorF position) const;
	const Vector2D<int> index(RectF rect) const;
	const Vector2D<int> index(const MapTile* tile) const;

	void renderBottomLayer(const TextureManager* tm, float yPoint);
	void renderTopLayer(const TextureManager* tm, float yPoint);

	bool wallRenderTile(Index index) const { return mData.get(index).renderType() >= MapTile::Wall; }
	bool floorRenderTile(Index index) const { return mData.get(index).renderType() == MapTile::Floor; }

	bool wallCollisionTile(Index index) const { return mData.get(index).collisionType() >= MapTile::Wall; }
	bool floorCollisionTile(Index index) const { return mData.get(index).collisionType() == MapTile::Floor; }
	bool floorCollisionTile(VectorF position) const { return mData.get(index(position)).collisionType() == MapTile::Floor; }

	bool isValidTile(RectF rect) const;
	bool isValidPosition(VectorF position) const;


private:
	void populateTileRects(VectorF offset);
	void populateCollisionRenderInfo();

	// Populate render tile info
	void topBottom();
	void leftRight();
	void corners();

	void renderColumn(const RectF& rect, Texture* column);
};