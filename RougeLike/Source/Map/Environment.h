#pragma once

#include "Events/Dispatcher.h"
#include "LevelManager.h"
#include "Actors/ActorManager.h"
#include "Objects/Effects/EffectPool.h"
#include "Items/Collectables/Collectables.h"
#include "Items/Collectables/CollectablesPool.h"

class Map;
class InputManager;


class Environment : public Dispatcher
{
public:
	Environment(GameData* gameData);

	void init();

	void load();
	void handleInput(const InputManager* input);
	void fastUpdate(float dt);
	void slowUpdate(float dt);

	void nextLevel();
	void restart();
	void exit();


	bool canClosePreviousLevel(VectorF playerPosition) const;

	void renderBottomLayer();
	void renderTopLayer();

	Map* map(VectorF position) const { return mLevelManager.map(position); }
	const Map* primaryMap() const { return mLevelManager.primaryMap(); }

	VectorF size() const { return mLevelManager.size(); }
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
	
	Collectables mCollectables;

	Vector2D<int> mMapSize;
};