#pragma once

#include "Map/MapBase.h"
#include "MapTile.h"

class TextureManager;
class Texture;


class Map : public MapBase<MapTile>
{
public:
	void init(Index index);
	void populateData(VectorF offset = VectorF());

	Vector2D<float> size() const;
	const VectorF tileSize() const;

	const RectF getFirstRect(int yIndex = 0) const;
	const RectF getLastRect(int yIndex = 0) const;

	const MapTile* tile(Index index) const { return MapBase<MapTile>::tile(index); };
	const MapTile* tile(VectorF position) const;

	const Vector2D<int> index(VectorF position) const;
	const Vector2D<int> index(RectF rect) const;
	const Vector2D<int> index(const MapTile* tile) const;

	const RectF tileRect(Index index) const;

	const MapTile* offsetTile(const MapTile* target, int xOffset, int yOffset) const;

	void renderBottomLayer(const TextureManager* tm, float yPoint);
	void renderTopLayer(const TextureManager* tm, float yPoint);

	bool wallRenderTile(Index index) const { return mData.get(index).renderType() >= MapTile::Wall; }
	bool floorRenderTile(Index index) const { return mData.get(index).renderType() == MapTile::Floor; }

	bool wallCollisionTile(Index index) const { return mData.get(index).collisionType() >= MapTile::Wall; }
	bool floorCollisionTile(Index index) const { return mData.get(index).collisionType() == MapTile::Floor; }

	void addTileType(Index index, MapTile::Type type);
	void setTileType(Index index, MapTile::Type type);
	void removeTileType(Index index, MapTile::Type type);

	const MapTile::EdgeInfo getEdgeInfo(Index index) const;

	Vector2D<int> findYFloorTileRange(int xTileIndex) const;

	bool isValidTile(RectF rect) const;
	bool isValidPosition(VectorF position) const;

#if DRAW_BINARY_MAP
	void printBinaryMap();
#endif
#if _DEBUG
	int tileRenderCounter = 0;
#endif

private:
	void populateTileRects(VectorF offset);
	void populateCollisionRenderInfo();


	void renderColumn(const RectF& rect, Texture* column);
};