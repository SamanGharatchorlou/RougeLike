#include "pch.h"
#include "TrapManager.h"

#include "Map/Map.h"
#include "Actors/Actor.h"

#include "Objects/Pools/EffectPool.h"



void TrapManager::init(Actor* actor, EffectPool* effectPool, const TrapDataMap* trapData) 
{ 
	mActor = actor; 
	mTrapData = trapData; 
	mEffectPool = effectPool;
}


void TrapManager::flushQueues()
{
	mUntriggeredTraps.clear();
	mTriggeredTraps.clear();
}


void TrapManager::slowUpdate()
{
	triggerTrap(mActor->position());
	updateTriggerTraps();
	updateResetTraps();
	processCollisions(mActor->position());
}


void TrapManager::triggerTrap(VectorF position)
{
	if (mMap->isValidPosition(position))
	{
		Index index = mMap->index(position);
		MapTile* tile = mMap->tile(index);

		// Add untriggered trap
		DecorType decor = tile->decorType();

		if (tile->has(DecorType::Spikes))
		{
			
			Animator& animator = tile->animation(0);

			if (animator.activeAimation()->currentFrame() == 0)
			{
				Trap trap(index, mTrapData->at(decor));
				mUntriggeredTraps.push(trap);
			}
		}
		else if (tile->has(DecorType::GratingTrigger))
		{
			Animator& animator = tile->animation(0);

			if (animator.activeAimation()->currentFrame() == 0)
			{
				animator.activeAimation()->nextFrame();
				triggerAll(DecorType::Grating);
			}
		}
	}
}


void TrapManager::triggerAll(DecorType type)
{
	for (unsigned int y = 0; y < mMap->yCount(); y++)
	{
		for (unsigned int x = 0; x < mMap->xCount(); x++)
		{
			Index tileIndex(x, y);
			MapTile* tile = mMap->tile(tileIndex);
			if (tile->is(type))
			{
				Trap trap(tileIndex, mTrapData->at(type));
				mPersistingTraps.push_back(trap);

				tile->animation(0).start();
			}
		}
	}
}


void TrapManager::processCollisions(VectorF position)
{
	Index index = mMap->index(position);

	for (std::deque<Trap>::iterator iter = mTriggeredTraps.begin(); iter != mTriggeredTraps.end(); iter++)
	{
		Trap& trap = *iter;
		if (trap.doesCollide(index))
		{
			resolveCollision(trap);
			break;
		}
	}

	for (int i = 0; i < mPersistingTraps.size(); i++)
	{
		Trap& trap = mPersistingTraps[i];
		if (trap.hasIndex(index) && !trap.isExhausted())
		{
			resolveCollision(trap);
			trap.reset();
			break;
		}
	}
}


void TrapManager::updateTriggerTraps()
{
	// Change trap state to un
	if (mUntriggeredTraps.size() > 0)
	{
		Trap& trap = mUntriggeredTraps.front();

		if (trap.canTrigger())
		{
			MapTile* tile = mapTile(trap);
			Animator& animator = tile->animation(0);

			if (animator.activeAimation()->currentFrame() == 0)
			{
				animator.activeAimation()->nextFrame();

				Trap& trap = mUntriggeredTraps.popFront();
				trap.reset();

				mTriggeredTraps.push(trap);
			}
		}
	}
}

void TrapManager::updateResetTraps()
{
	// Pop traps from queue after time > cooldown
	if (mTriggeredTraps.size() > 0)
	{
		Trap& trap = mTriggeredTraps.front();
		if (trap.canUntrigger())
		{
			MapTile* tile = mapTile(trap);
			Animator& animator = tile->animation(0);

			if (animator.activeAimation()->currentFrame() == 1)
			{
				animator.activeAimation()->nextFrame();
				mTriggeredTraps.popFront();
			}
		}
	}

	// Reset persisting traps after time > cooldown
	for (int i = 0; i < mPersistingTraps.size(); i++)
	{
		Trap& trap = mPersistingTraps[i];
		if (trap.canUntrigger())
		{
			trap.setExhausted(false);
		}
	}
}



MapTile* TrapManager::mapTile(Trap trap) const
{
	Index trapIndex = trap.index();
	return mMap->tile(trapIndex);
}

void TrapManager::resolveCollision(Trap& trap)
{
	Effect* damageEffect = mEffectPool->getObject(EffectType::Damage);
	damageEffect->fill(trap.properties());

	mActor->addEffect(damageEffect);
	trap.setExhausted(true);
}
