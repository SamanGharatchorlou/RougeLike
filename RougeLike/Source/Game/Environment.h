#pragma once

#include "Events/Dispatcher.h"
#include "Map/LevelManager.h"
#include "Actors/ActorManager.h"
#include "Objects/Pools/EffectPool.h"
#include "Items/Collectables/Collectables.h"


class Map;
class InputManager;


class Environment : public Dispatcher
{
public:
	void init(GameData* gameData);

	void load();
	void handleInput(const InputManager* input);
	void fastUpdate(float dt);
	void slowUpdate(float dt);

	void nextLevel();
	void clear();


	bool canClosePreviousLevel(VectorF playerPosition) const;

	void renderBottomLayer();
	void renderTopLayer();

	Map* map(VectorF position) const { return mLevelManager.map(position); }
	Map* map(MapType type) const { return mLevelManager.map(type); }
	//const Map* primaryMap() const { return mLevelManager.primaryMap(); }

	//VectorF size() const { return mLevelManager.size(); }
	int mapLevel() const { return mLevelManager.level(); }

	void setCameraBoundaries();
	VectorF toWorldCoords(VectorF cameraCoords);

	ActorManager* actors() { return &mActors; }
	EffectPool* effectPool() { return &mEffectPool; }



private:
	// Make these pointers to place .h files into this.cpp?
	ActorManager mActors;
	LevelManager mLevelManager;
	EffectPool mEffectPool;
	
	// MOVE to level manager
	Collectables mCollectables;

	Vector2D<int> mMapSize;
};