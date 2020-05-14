#pragma once

#include "Events/Dispatcher.h"

class Map;
class TextureManager;

class Environment : public Dispatcher
{
public:
	Environment();

	void init();
	void restart();

	void nextLevel();
	void closeLevelEntrace();

	void renderBottomLayer(const TextureManager* tm, float depth);
	void renderTopLayer(const TextureManager* tm, float depth);

	Map* map(VectorF position) const;
	Map* primaryMap() const { return mPrimaryMap; }

	VectorF size() const;

	bool closeEntrance(VectorF position) const;
	bool generateNextLevel(VectorF position) const;

	RectF boundaries() const;

	int mapLevel() const { return mMapLevel; }

	VectorF toWorldCoords(VectorF cameraCoords);

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
	Map* mPrimaryMap;
	Map* mExit;

	Vector2D<int> mMapSize;

	// actor map?

	int mMapLevel;
};