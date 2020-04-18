#pragma once

#include "Map/MapBase.h"
#include "MapTile.h"

class TextureManager;
class Texture;

class Map : public MapBase<MapTile>
{
public:
	void init(int x, int y);
	void populateData(VectorF offset = VectorF());

	Vector2D<float> size() const;
	const VectorF tileSize() const;

	const RectF getFirstRect(int yIndex = 0) const;
	const RectF getLastRect(int yIndex = 0) const;

	const MapTile* tile(Index index) const { return MapBase<MapTile>::tile(index); };
	const MapTile* tile(int x, int y) const { return MapBase<MapTile> ::tile(x, y); }
	const MapTile* tile(VectorF position) const;

	const Vector2D<int> index(VectorF position) const;
	const Vector2D<int> index(const MapTile* tile) const;
	const Vector2D<int> index(RectF rect) const;

	const RectF tileRect(Index index) const;
	const RectF tileRect(int x, int y) const;

	const MapTile* offsetTile(const MapTile* target, int xOffset, int yOffset) const;

	void renderBottomLayer(const TextureManager* tm, float yPoint);
	void renderTopLayer(const TextureManager* tm, float yPoint);

	bool wallRenderTile(int x, int y) const { return mData.get(x, y).renderType() >= MapTile::Wall; }
	bool floorRenderTile(int x, int y) const { return mData.get(x, y).renderType() == MapTile::Floor; }

	bool wallCollisionTile(int x, int y) const { return mData.get(x, y).collisionType() >= MapTile::Wall; }
	bool floorCollisionTile(int x, int y) const { return mData.get(x, y).collisionType() == MapTile::Floor; }

	void addTileType(int x, int y, MapTile::Type type);
	void setTileType(int x, int y, MapTile::Type type);
	void removeTileType(int x, int y, MapTile::Type type);

	const MapTile::EdgeInfo getEdgeInfo(int x, int y) const;

	Vector2D<int> findYFloorTileRange(int xTileIndex);

#if DRAW_BINARY_MAP
	void printBinaryMap();
#endif
#if _DEBUG
	int tileRenderCounter = 0;
#endif

private:
	void populateTileRects(VectorF offset);
	void populateCollisionRenderInfo();

	bool isValidTile(RectF rect) const;
	bool isValidPosition(VectorF position) const;

	void renderColumn(const RectF& rect, Texture* column);
};