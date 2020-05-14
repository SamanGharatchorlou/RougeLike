#pragma once

#include "CollisionTracker.h"

class Collider;

class CollisionManager
{
public:
	enum Tracker
	{
		// Slow Updates
		Cursor_Actors,

		fast_updates,

		// Fast Updates
		PlayerWeapon_Hit_Enemy,
		Enemy_Hit_Player,
		Player_Hit_Collectable,
	};


public:
	void fastUpdate();
	void slowUpdate();

	void resetColliders();
	void clearColliders();

	CollisionTracker* getTracker(Tracker id);
	void addNewCollisionTracker(Tracker id);

	void addAttackers(Tracker id, std::vector<Collider*> attackers);
	void addDefenders(Tracker id, std::vector<Collider*> defenders);

	void removeDefender(Tracker id, Collider* defender);
	void removeAllDefenders(Tracker id);
	void removeAllAttackers(Tracker id);



private:
	std::vector<std::pair<Tracker, CollisionTracker>> mCollisionTrackers;


};