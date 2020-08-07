#pragma once

#include "Collisions/EffectCollider.h"
#include "Objects/Properties/PropertyTypes/Damage.h"


class CollisionTracker;
class Map;


class TrapManager
{
public:
	class Trap
	{
	public:
		Trap(Index index) : mIndex(index), timer(TimerF::Start), exhausted(false) { }

		inline bool operator == (Trap value)
		{
			return is(value.index());
		}

		Index index() const { return mIndex; }
		bool is(Index index) const { return index == mIndex; }

		bool isExhausted() const { return exhausted; }
		void exhaust() { exhausted = true; }

		float time() const { return timer.getSeconds(); }
		void reset() { timer.restart(); }

	private:
		Index mIndex;
		TimerF timer;
		bool exhausted;
	};


public:
	TrapManager(Map* map) : mMap(map), mTriggerTime(0.75f), mRecoveryTime(1.0f) { }
	void set(Damage damage, float triggerTime, float recoveryTime);


	void flushQueues();

	void slowUpdate();

	void triggerTrap(VectorF position);

	bool didCollide(VectorF position);

	Damage damage() const { return mDamage; }

	
private:
	void updateTriggerTraps();
	void updateResetTraps();


private:
	Map* mMap;
	UniqueQueue<Trap> mUntriggeredTraps;
	std::deque<Trap> mTriggeredTraps;

	float mTriggerTime;
	float mRecoveryTime;

	Damage mDamage;
};