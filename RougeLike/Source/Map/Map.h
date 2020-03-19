#pragma once
#include "MapTile.h"

class TextureManager;

class Map
{
public:
	Map();

	void init(int x, int y);
	void populateData(VectorF offset = VectorF());

	Grid<MapTile>& getData() { return mData; }

	unsigned int yCount() const { return mTileCount.y; }
	unsigned int xCount() const { return mTileCount.x; }

	const RectF getFirstRect(int yIndex = 0) const;
	const RectF getLastRect(int yIndex = 0) const;

	const MapTile* getTile(Vector2D<int> index) const;
	const MapTile* getTile(int x, int y) const;
	const MapTile* getTile(VectorF position) const;

	const Vector2D<int> getIndex(VectorF position) const;
	const Vector2D<int> getIndex(const MapTile* tile) const;
	const Vector2D<int> getIndex(RectF rect) const;

	const RectF getTileRect(Vector2D<int> index) const;
	const RectF getTileRect(int x, int y) const;

	const VectorF getTileSize() const { return mTileSize * mScale; }

	const MapTile* offsetTile(const MapTile* tile, int xOffset, int yOffset) const;

	Vector2D<float> size() const;

	void setScale(float scale) { mScale = scale; }
	float getScale() const { return mScale; }

	void renderLayerA(const TextureManager* tm, float yPoint);
	void renderLayerB(const TextureManager* tm, float yPoint);

	Vector2D<int> findYFloorTileRange(int xTileIndex);

	void setUnpassableTile(Vector2D<int> index);

	bool wallRenderTile(int x, int y) const { return mData.get(x, y).renderType() >= MapTile::Wall; }
	bool floorRenderTile(int x, int y) const { return mData.get(x, y).renderType() == MapTile::Floor; }

	bool wallCollisionTile(int x, int y) const { return mData.get(x, y).collisionType() >= MapTile::Wall; }
	bool floorCollisionTile(int x, int y) const { return mData.get(x, y).collisionType() == MapTile::Floor; }

	void addColumn(int x, int y);

	bool inBounds(int x, int y) const;

	const MapTile::EdgeInfo getEdgeInfo(int x, int y) const;

#if DRAW_BINARY_MAP
	void printBinaryMap();
#endif


private:
	void populateTileRects(VectorF offset);
	void populateCollisionRenderInfo();

	bool isValidTile(RectF rect) const;
	bool isValidIndex(Vector2D<int> index) const;
	bool isValidPosition(VectorF position) const;


	void renderTile(const RectF& rect);


private:
	Vector2D<int> mTileSize;
	Vector2D<int> mTileCount;

	float mScale;

	Grid<MapTile> mData;

#if _DEBUG
	int tileRenderCounter;
#endif
};