#pragma once


#include "WallCollisionTracker.h"
#include "WeaponCollisionManager.h"
#include "CollisionManager.h"

class CollisionManager;
class Player;
class Map;
class EffectPool;


class PlayerCollisions
{
public:
	PlayerCollisions() { };

	void init(Player* player, CollisionManager* collisionManager);

	void resolveWalls(Map* map, float dt);
	void resolveTraps(Map* map, EffectPool* effects);
	void resolveWeapons(EffectPool* effects);
	void resolveBody();


	void refreshWeaponColliders();

	void clearExcludedColliders(CollisionManager::Tracker tracker);
	void enableCollisions(CollisionManager::Tracker tracker, bool isEnabled);


	Player* player() { return mPlayer; }
	CollisionManager* collisionManager() { return cManager; }


private:
	void addCollidersToTrackers();
	void resolveTrapCollisions(Map* map, EffectPool* effects);


private:
	Player* mPlayer;
	CollisionManager* cManager;

	WallCollisionTracker mWallCollisions;
	WeaponCollisionManager mWeaponCollisions;
};