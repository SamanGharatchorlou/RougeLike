#pragma once

#include "Events/Dispatcher.h"
#include "LevelManager.h"
#include "Actors/ActorManager.h"
#include "Actors/Enemies/Spawning/EnemySpawner.h"

class Map;
struct GameData;


class Environment : public Dispatcher
{
public:
	Environment(GameData* gameData);

	void init();
	void handleInput();
	void fastUpdate(float dt);
	void slowUpdate(float dt);

	void nextLevel();
	void restart();
	void exit();


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

	ActorManager* actors() { return &mActors; }


private:
	void createNewMaps();
	void readConfigData(Vector2D<int>& mapIndexSize, VectorF& tileSize, float& scale);


private:
	ActorManager mActors;
	LevelManager mLevelManager;
	EnemySpawner mSpawner;

	Map* mEntrace;
	Map* mPrimaryMap;
	Map* mExit;

	Vector2D<int> mMapSize;
};