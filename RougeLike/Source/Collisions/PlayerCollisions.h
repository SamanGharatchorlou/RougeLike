#pragma once


#include "WallCollisionTracker.h"
#include "WeaponCollisionManager.h"
#include "CollisionManager.h"

class CollisionManager;
class Player;
class Map;


class PlayerCollisions
{
public:
	PlayerCollisions() { };

	void init(Player* player, CollisionManager* collisionManager);
	void fastUpdate(float dt, Map* map);
	void slowUpdate(Map* map);

	void refreshWeaponColliders();

	void clearExcludedColliders(CollisionManager::Tracker tracker);
	void enableCollisions(CollisionManager::Tracker tracker, bool isEnabled);


	Player* player() { return mPlayer; }
	CollisionManager* collisionManager() { return cManager; }


private:
	void addCollidersToTrackers();
	void updateWeaponColliders();
	void resolveTrapCollisions(Map* map);


private:
	Player* mPlayer;
	CollisionManager* cManager;

	WallCollisionTracker mWallCollisions;
	WeaponCollisionManager mWeaponCollisions;
};