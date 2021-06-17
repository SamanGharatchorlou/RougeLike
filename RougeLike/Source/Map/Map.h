#pragma once

#include "Map/MapBase.h"
#include "Tiles/MapTile.h"
#include "TrapManager.h"

class Camera;
class Actor;
class EffectPool;

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
	Map();
	Map(Vector2D<int> mapIndexSize);

	void initTrapManager(Actor* actor, EffectPool* effectPool, const TrapDataMap* trapData);
	void setInfo(MapType type, int level) { mType = type; mLevel = level; }

	MapType type() const { return mType; }

	void buildDeferredRenderList();

	// Update
	void slowUpdate(float dt);
	void pause() { mTraps.pause(); }
	void resume() { mTraps.resume(); }

	// Rendering
	void renderFloor() const;
	void renderLowerLayer() const;
	void renderUpperLayer() const;
	void deferredRender() const;
    
    void fillRenderQueue_Floor();
    void fillRenderQueue_LowDepth();
    void fillRenderQueue_HighDepth();

	void setSize(Vector2D<int> size);

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

	const MapTile* randomFloorTile(int xPointPercentage) const;
	const MapTile* randomFloorTile() const;

	bool isValidTile(RectF rect) const;
	bool isValidPosition(VectorF position) const;


private:
	void render(const MapTile* tile, const Camera* camera) const;
    void fillRenderQueue(const MapTile* tile, const Camera* camera);

private:
	MapType mType;
	TrapManager mTraps;
	std::vector<MapTile*> mDeferredRendering;
};
