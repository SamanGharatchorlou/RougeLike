#pragma once

#include "CollisionTracker.h"

class Collider;


class CollisionManager
{
public:
	enum Tracker
	{
		PlayerWeapon_Hit_Enemy,
		Player_Hit_Enemy,
		Player_Hit_Collectable,
		Enemy_Hit_Player,
		Count
	};

	using TrackerMap = std::unordered_map<CollisionManager::Tracker, CollisionTracker*>;


public:

	~CollisionManager();
	void init();
	void fastUpdate();

	void resetColliders();
	void clearColliders();

	CollisionTracker* getTracker(Tracker id);
	void addCollisionTracker(Tracker id); // TODO: make private?
	//void addComplexCollisionTracker(Tracker id);

	void addAttackers(Tracker id, std::vector<Collider*> attackers);
	void addDefenders(Tracker id, std::vector<Collider*> defenders);

	void removeDefender(Tracker id, const Collider* defender);
	void removeAllDefenders(Tracker id);
	void removeAllAttackers(Tracker id);


private:
	TrackerMap mTrackers;

};
