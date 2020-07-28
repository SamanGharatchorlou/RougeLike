#pragma once

#include "Events/Dispatcher.h"
#include "LevelManager.h"
#include "Actors/ActorManager.h"
#include "Actors/Enemies/Spawning/EnemySpawner.h"
#include "Objects/Effects/EffectPool.h"

class Map;
struct GameData;
class Cursor;


class Environment : public Dispatcher
{
public:
	struct Maps
	{
		Map* entrance;
		Map* primaryMap;
		Map* exit;
	};

public:
	Environment(GameData* gameData);

	void init();

	void load();
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
	Map* primaryMap() const { return mMaps.primaryMap; }

	VectorF size() const;
	int mapLevel() const { return mLevelManager.level(); }

	void setCameraBoundaries();
	VectorF toWorldCoords(VectorF cameraCoords);

	ActorManager* actors() { return &mActors; }
	EffectPool* effectPool() { return &mEffectPool; }


private:
	void createNewMaps();
	void readConfigData(Vector2D<int>& mapIndexSize, VectorF& tileSize, float& scale);


private:
	ActorManager mActors;
	LevelManager mLevelManager;
	EnemySpawner mSpawner;
	EffectPool mEffectPool;

	Maps mMaps;

	Vector2D<int> mMapSize;
};