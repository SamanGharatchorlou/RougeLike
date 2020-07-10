#pragma once


#include "Collisions/WallCollisionTracker.h"
#include "Map/TrapManager.h"

class CollisionManager;
class Player;
class Map;



class PlayerCollisions
{
public:
	PlayerCollisions(Player* player, CollisionManager* collisionManager);

	void triggerTraps(Map* map);
	void resolveTrapCollisions(const Map* map);

	void resolveWallCollisions(const Map* map, float dt);
	void updateWeaponColliders();

	void addColliderToTrackers();
	void refreshWeaponColliders();

	void clearExcludedList();
	void addEnemiesToExcludedList();

	void enableBodyCollisions(bool isEnabled);
	void enableWeaponCollisions(bool isEnabled);


private:
	struct IndexTimer
	{
		IndexTimer(Index i, TimerF time) : index(i), timer(time) { }

		inline bool operator == (IndexTimer value)
		{
			return index == value.index;
		}

		Index index;
		TimerF timer;
	};


private:
	Player* mPlayer;
	CollisionManager* cManager;

	WallCollisionTracker mWallCollisions;

	TrapManager mTrapManager;
};