#pragma once
#include "MapTile.h"

struct GameData;

// TODO: replace name with TileMap
class Map
{
public:
	Map(GameData* gameData);

	void generateRandomTunnel(int y, int x);
	void populateTileRects();

	void setScale(float scale);

	unsigned int yCount() const { return mTileCount.y; }
	unsigned int xCount() const { return mTileCount.x; }

	const MapTile& getTile(Vector2D<int> index) const;
	const MapTile& getTile(int x, int y) const;
	const MapTile& getTile(VectorF position) const;

	Vector2D<int> getIndex(VectorF position) const;
	Vector2D<int> getIndex(MapTile tile) const;
	Vector2D<int> getIndex(RectF rect) const;

	RectF getTileRect(Vector2D<int> index) const;
	RectF getTileRect(int x, int y) const;
	RectF getTileRect(int coords[2]) const;

	VectorF getTileSize() { return mTileSize * mScale; }

	const MapTile& offsetTile(MapTile tile, int xOffset, int yOffset) const;

	Vector2D<float> size() const;
	float getScale() { return mScale; }

	void renderLayerA(float yPoint);
	void renderLayerB();

#if DRAW_BINARY_MAP
	void printBinaryMap();
#endif

	Vector2D<int> findYFloorTileRange(int xTileIndex);

	bool wallRenderTile(int x, int y) { return mData[y][x].renderType() >= MapTile::Wall; }
	bool floorRenderTile(int x, int y) { return mData[y][x].renderType() == MapTile::Floor; }

	bool wallCollisionTile(int x, int y) { return mData[y][x].collisionType() >= MapTile::Wall; }
	bool floorCollisionTile(int x, int y) { return mData[y][x].collisionType() == MapTile::Floor; }

	bool inBounds(int x, int y) { return ((x >= 0 && x < mTileCount.x) && (y >= 0 && y < mTileCount.y)); }

	MapTile::EdgeInfo getEdgeInfo(int x, int y);

#if _DEBUG
	Grid <MapTile>& getData() { return mData; }
#endif

private:
	// TODO: make these annoymous?
	bool isValidTile(RectF rect) const;
	bool isValidIndex(Vector2D<int> index) const;
	bool isValidPosition(VectorF position) const;

private:
	GameData* mGameData;

	Vector2D<int> mTileSize;
	Vector2D<int> mTileCount;

	float mScale;

	Grid<MapTile> mData;

	float yLayerA;

#if _DEBUG
	int tileRenderCounter;
#endif
};