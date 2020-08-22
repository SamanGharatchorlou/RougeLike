#pragma once

#include "CollisionTracker.h"

class Collider;


class CollisionManager
{
public:
	enum Tracker
	{
		PlayerWeapon_Hit_Enemy,
		Player_Hit_Collectable,

		Enemy_Hit_Player,
		
		Player_Trigger_Trap,
		Trap_Hit_Player,

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

	void addAttacker(Tracker id, Collider* attacker);
	void addAttackers(Tracker id, std::vector<Collider*> attackers);
	void addDefender(Tracker id, Collider* defender);
	void addDefenders(Tracker id, std::vector<Collider*> defenders);


	void removeAttacker(Tracker id, const Collider* attacker);
	void removeDefender(Tracker id, const Collider* defender);
	void removeAllDefenders(Tracker id);
	void removeAllAttackers(Tracker id);

#if _DEBUG
	int defenderCount(Tracker id);
#endif

private:
	void addCollisionTracker(Tracker id);


private:
	TrackerMap mTrackers;

};
