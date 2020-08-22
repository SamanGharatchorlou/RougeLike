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
	void clear();

	void resolveWalls(Map* map, float dt);
	void resolveWeapons(EffectPool* effects);

	void refreshWeaponColliders();
	void clearExcludedColliders(CollisionManager::Tracker tracker);


	Player* player() { return mPlayer; }
	CollisionManager* collisionManager() { return cManager; }


private:
	void enableCollisions(CollisionManager::Tracker tracker, bool isEnabled);


private:
	void addCollidersToTrackers();


private:
	Player* mPlayer;
	CollisionManager* cManager;

	WallCollisionTracker mWallCollisions;
	WeaponCollisionManager mWeaponCollisions;
};