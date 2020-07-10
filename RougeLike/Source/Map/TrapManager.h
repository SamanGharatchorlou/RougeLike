#pragma once

#include "Collisions/EffectCollider.h"


class CollisionTracker;
class Map;


class TrapManager
{
public:
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


public:
	TrapManager() { }

	void addTrapColliders(Map* map, CollisionTracker* tracker);

	void slowUpdate(Map* map);

	void triggerTrap(Map* map, VectorF position);

	
private:
	void updateTriggerTraps(Map* map);
	void updateResetTraps(Map* map);


private:
	UniqueQueue<IndexTimer> mUntriggeredTraps;
	std::queue<IndexTimer> mTriggeredTraps;
};