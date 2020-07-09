#pragma once


#include "Collisions/WallCollisionTracker.h"

class CollisionManager;
class Player;
class Map;


class PlayerCollisions
{
public:
	PlayerCollisions(Player* player, CollisionManager* collisionManager);

	void updateTraps(Map* map);
	void triggerTraps(Map* map);

	void resolveWallCollisions(const Map* map, float dt);
	void updateWeaponColliders();

	void addColliderToTrackers();
	void refreshWeaponColliders();

	void clearExcludedList();
	void addEnemiesToExcludedList();

	void enableBodyCollisions(bool isEnabled);
	void enableWeaponCollisions(bool isEnabled);


private:


private:
	Player* mPlayer;
	CollisionManager* cManager;

	WallCollisionTracker mWallCollisions;

	std::queue<std::pair<Index, TimerF>> mUntriggeredTraps;
	std::queue<std::pair<Index, TimerF>> mTriggeredTraps;
};