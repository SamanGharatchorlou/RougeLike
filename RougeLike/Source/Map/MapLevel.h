#pragma once

#include "Events/Dispatcher.h"

class Map;

class MapLevel : public Dispatcher
{
public:
	MapLevel();

	void init();

	void nextLevel();
	void closeLevelEntrace();

	void renderBottomLayer(const TextureManager* tm, float depth);
	void renderTopLayer(const TextureManager* tm, float depth);

	Map* map(VectorF position = VectorF(-1.0f, -1.0f)) const;
	Map* primaryMap() const { return mMap; }

	VectorF size() const;

	bool closeEntrance(VectorF position) const;
	bool generateNextLevel(VectorF position) const;

	RectF boundaries() const;

private:
	void buildEntrance(float offset);
	void buildLevel();
	void buildExit();

	void swapToEntrance();
	void swapToExit();

	void closeEntrance();
	void closeLevel();

	void buildRandomLevel(int width, int height, float offset);


private:
	Map* mEntrace;
	Map* mMap;
	Map* mExit;

	Vector2D<int> mapSize;
};