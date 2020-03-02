#pragma once
#include "MapTile.h"


struct GameData;


class Map
{
public:
	Map(GameData* gameData);

	void generateRandomTunnel(int y, int x);
	void populateTileRects();

	unsigned int yCount() const { return mTileCount.y; }
	unsigned int xCount() const { return mTileCount.x; }

	const MapTile* getTile(Vector2D<int> index) const;
	const MapTile* getTile(int x, int y) const;
	const MapTile* getTile(VectorF position) const;

	const Vector2D<int> getIndex(const VectorF position) const;
	const Vector2D<int> getIndex(const MapTile* tile) const;
	const Vector2D<int> getIndex(const RectF rect) const;

	const RectF getTileRect(Vector2D<int> index) const;
	const RectF getTileRect(int x, int y) const;
	const RectF getTileRect(int coords[2]) const;

	const VectorF getTileSize() const { return mTileSize * mScale; }

	const MapTile* offsetTile(const MapTile* tile, int xOffset, int yOffset) const;

	Vector2D<float> size() const;

	void setScale(float scale) { mScale = scale; }
	float getScale() const { return mScale; }

	void renderLayerA(float yPoint);
	void renderLayerB();

	Vector2D<int> findYFloorTileRange(int xTileIndex);

	bool wallRenderTile(int x, int y) const { return mData.get(y,x).renderType() >= MapTile::Wall; }
	bool floorRenderTile(int x, int y) const { return mData.get(y, x).renderType() == MapTile::Floor; }
	bool wallCollisionTile(int x, int y) const { return mData.get(y, x).collisionType() >= MapTile::Wall; }
	bool floorCollisionTile(int x, int y) const { return mData.get(y, x).collisionType() == MapTile::Floor; }

	bool inBounds(int x, int y) const;

	const MapTile::EdgeInfo getEdgeInfo(int x, int y) const;

#if DRAW_BINARY_MAP
	void printBinaryMap();
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