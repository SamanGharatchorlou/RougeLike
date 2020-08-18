#pragma once

#include "Objects/Pools/TrapPool.h"

class Map;
class MapTile;
class Trap;
class Trigger;
class DecorMap;
class CollisionManager;
class EffectPool;

class TileInteractableManager
{
public:
	TileInteractableManager() : mCollisions(nullptr), mEffects(nullptr) { };

	void init(CollisionManager* collisions, EffectPool* effects);
	void load();
	void clear();

	void addMap(Map* map, const DecorMap& info);
	void popFrontMap();

	void slowUpdate();

	
private:
	void prepare(TileInteractable* interactable, MapTile* tile, const DecorMap& info);

	void set(Trap* trap);
	void trigger(TileInteractable* interactable);
	void reset(Trap* trap);


private:
	CollisionManager* mCollisions;

	EffectPool* mEffects;

	TrapPool mTrapPool;

	struct Interactables
	{
		void init()
		{
			traps = new std::vector<Trap*>;
			triggers = new std::vector<Trigger*>;
		}

		void free()
		{
			delete traps;
			delete triggers;
		}

		std::vector<Trap*>* traps;
		std::vector<Trigger*>* triggers;
	};

	std::vector<Interactables> mInteractables;
};