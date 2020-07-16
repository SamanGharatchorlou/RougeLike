#include "pch.h"
#include "TrapManager.h"

#include "Map.h"


void TrapManager::set(float triggerTime, float recoveryTime)
{
	mTriggerTime = triggerTime;
	mRecoveryTime = recoveryTime;
}

void TrapManager::flushQueues()
{
	mUntriggeredTraps.clear();
	mTriggeredTraps.clear();
}

void TrapManager::slowUpdate()
{
	updateTriggerTraps();
	updateResetTraps();
}

void TrapManager::triggerTrap(VectorF position)
{
	Index index = mMap->index(position);
	MapTile* tile = mMap->tile(index);

	// Add untriggered trap
	if (tile->has(DecorType::Spikes))
	{
		Animator& animator = tile->animation(0);

		if (animator.animation().currentFrame() == 0)
		{
			Trap trap(index);
			mUntriggeredTraps.push(trap);
		}
	}
}


bool TrapManager::didCollide(VectorF position)
{
	Index index = mMap->index(position);

	for (std::deque<Trap>::iterator iter = mTriggeredTraps.begin(); iter != mTriggeredTraps.end(); iter++)
	{
		Trap& trap = *iter;
		if (trap.is(index) && !trap.isExhausted())
		{
			trap.exhaust();
			return true;
		}
	}

	return false;
}


void TrapManager::updateTriggerTraps()
{
	if (mUntriggeredTraps.size() > 0 && mUntriggeredTraps.front().time() > mTriggerTime)
	{
		Index trapIndex = mUntriggeredTraps.front().index();
		MapTile* tile = mMap->tile(trapIndex);
		Animator& animator = tile->animation(0);

		// TODO: this is being triggered everyframe, make it only once
		if (animator.animation().currentFrame() == 0)
		{
			animator.getAnimation(0).nextFrame();

			Trap& trap = mUntriggeredTraps.popFront();
			trap.reset();

			mTriggeredTraps.push_back(trap);
		}
	}
}

void TrapManager::updateResetTraps()
{
	if (mTriggeredTraps.size() > 0 && mTriggeredTraps.front().time() > mRecoveryTime)
	{
		Index trapIndex = mTriggeredTraps.front().index();
		MapTile* tile = mMap->tile(trapIndex);
		Animator& animator = tile->animation(0);

		if (animator.animation().currentFrame() == 1)
		{
			animator.getAnimation(0).nextFrame();
			mTriggeredTraps.pop_front();
		}
	}
}