#pragma once

#include "Map/MapBase.h"
#include "Tiles/MapTile.h"

class Camera;


enum class MapType
{
	None,
	Dungeon,
	Corridor,
	End
};


class Map : public MapBase<MapTile>
{
public:
	Map() : mType(MapType::None) { }
	Map(Vector2D<int> mapIndexSize);
	~Map() { };

	void setType(MapType type) { mType = type; }
	MapType type() const { return mType; }

	void buildDeferredRenderList();

	// Update
	void slowUpdate(float dt);

	// Rendering
	void renderFloor();
	void renderLowerLayer();
	void renderUpperLayer();
	void deferredRender();

	void setSize(Vector2D<int> size);
	void clearData();

	// Getters
	VectorF tileSize() const;
	VectorF size() const;

	const RectF getFirstRect() const;
	const RectF getLastRect() const;
	const RectF getBottomLastRect() const;

	VectorF midPoint() const;

	MapTile* tile(Index index) { return &mData[index]; };
	const MapTile* tile(Index index) const { return &mData.get(index); };
	const MapTile* tile(VectorF position) const;

	const Vector2D<int> index(VectorF position) const;
	const Vector2D<int> index(RectF rect) const;
	const Vector2D<int> index(const MapTile* tile) const;

	Vector2D<int> yTileFloorRange(VectorF position) const;
	const MapTile* randomFloorTile(int xPointPercentage) const;

	bool isValidTile(RectF rect) const;
	bool isValidPosition(VectorF position) const;

private:

	void render(MapTile* tile, Camera* camera);


private:
	MapType mType;

	std::vector<MapTile*> mDeferredRendering;
};