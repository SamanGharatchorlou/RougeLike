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
	~Environment();

	void init(GameData* gameData);

	void load();
	void handleInput(const InputManager* input);
	void fastUpdate(float dt);
	void slowUpdate(float dt);

	void pause() { mLevelManager.pause(); }
	void resume() { mLevelManager.resume(); }

	void clear();

	void renderFloor();
	void renderBottomLayer();
	void renderTopLayer();

	Map* map(VectorF position) const { return mLevelManager.map(position); }
	Map* firstMap(MapType type) const { return mLevelManager.firstMap(type); }
	Map* lastMap(MapType type) const { return mLevelManager.lastMap(type); }

	int mapLevel() const { return mLevelManager.level(); }

	void setCameraBoundaries();
	VectorF toWorldCoords(VectorF cameraCoords);

	ActorManager* actors() { return &mActors; }
	EffectPool* effectPool() { return &mEffectPool; }
	LevelManager* levelManager() { return &mLevelManager; }


private:
	ActorManager mActors;
	LevelManager mLevelManager;
	EffectPool mEffectPool;
	
	// MOVE to level manager
	Collectables mCollectables;

	Vector2D<int> mMapSize;
};