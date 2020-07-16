#pragma once

#include "Events/Dispatcher.h"
#include "LevelManager.h"
#include "Actors/Enemies/Spawning/EnemySpawner.h"

class Map;
class TextureManager;


class Environment : public Dispatcher
{
public:
	Environment(TextureManager* textureManager);

	void init();
	void slowUpdate(float dt);
	void restart();

	void nextLevel();

	bool canClosePreviousLevel(VectorF playerPosition) const;

	void renderFloor();
	void renderBottomLayer();
	void renderTopLayer();

	Map* map(VectorF position) const;
	Map* primaryMap() const { return mPrimaryMap; }

	VectorF size() const;
	int mapLevel() const { return mLevelManager.level(); }

	void setCameraBoundaries();
	VectorF toWorldCoords(VectorF cameraCoords);


private:
	void createNewMaps();
	void readConfigData(Vector2D<int>& mapIndexSize, VectorF& tileSize, float& scale);


private:
	LevelManager mLevelManager;
	EnemySpawner mSpawner;

	Map* mEntrace;
	Map* mPrimaryMap;
	Map* mExit;

	Vector2D<int> mMapSize;
};