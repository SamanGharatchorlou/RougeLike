#include "pch.h"
#include "TrapManager.h"

#include "Map/Map.h"
#include "Actors/Actor.h"

#include "Objects/Pools/EffectPool.h"

#include "Audio/AudioManager.h"


TrapManager::~TrapManager()
{
	if (AudioManager::Get()->isPlaying("Fire", this))
		AudioManager::Get()->stop("Fire", this);

	mUntriggeredTraps.clear();
	mTriggeredTraps.clear();
	mPersistingTraps.clear();

	mEffectPool = nullptr;
	mActor = nullptr;
	mTrapData = nullptr;
}


void TrapManager::init(Actor* actor, EffectPool* effectPool, const TrapDataMap* trapData) 
{ 
	mActor = actor; 
	mTrapData = trapData; 
	mEffectPool = effectPool;
}


void TrapManager::slowUpdate()
{
	if (mMap->isValidPosition(mActor->position()))
	{
		Index index = mMap->index(mActor->position());
		processCollisions(index);
		triggerTrap(index);
	}

	updateTriggerTraps();
	updateResetTraps();
}


void TrapManager::triggerTrap(Index index)
{
	MapTile* tile = mMap->tile(index);
	if (tile->has(DecorType::Spikes))
	{
		Animator& animator = tile->animation(0);

		if (animator.activeAimation()->currentFrame() == 0)
		{
			Trap trap(index, mTrapData->at(tile->decorType()));

			if (!mUntriggeredTraps.contains(trap))
			{
				mUntriggeredTraps.push(trap);

				VectorF position = mMap->tile(index)->rect().Center();
				AudioManager::Get()->play("TimerClick", nullptr, position); // Click trigger sound
			}
		}
	}
	else if (tile->has(DecorType::GratingTrigger))
	{
		Animator& animator = tile->animation(0);

		if (animator.activeAimation()->currentFrame() == 0)
		{
			animator.activeAimation()->nextFrame();
			triggerAll(DecorType::Grating);

			AudioManager::Get()->play("StoneTrigger", nullptr); // Click trigger sound
		}
	}
}


void TrapManager::triggerAll(DecorType type)
{
	VectorF triggerPosition;
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

			if (tile->is(DecorType::GratingTrigger))
			{
				triggerPosition = tile->rect().Center();
			}
		}
	}

	AudioManager::Get()->play("Fire", this, triggerPosition);
}


void TrapManager::processCollisions(Index index)
{
	for (UniqueQueue<Trap>::iterator iter = mTriggeredTraps.begin(); iter != mTriggeredTraps.end(); iter++)
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


void TrapManager::pause()
{
	AudioManager* audio = AudioManager::Get();

	if (audio->isPlaying("TimerClick", nullptr))
		audio->pause("TimerClick", nullptr);

	if (audio->isPlaying("Fire", this))
		audio->pause("Fire", this);

	for (UniqueQueue<Trap>::iterator iter = mTriggeredTraps.begin(); iter != mTriggeredTraps.end(); iter++)
	{
		iter->pause();
	}

	for (UniqueQueue<Trap>::iterator iter = mUntriggeredTraps.begin(); iter != mUntriggeredTraps.end(); iter++)
		iter->pause();

}
void TrapManager::resume()
{
	AudioManager* audio = AudioManager::Get();

	if (audio->isPlaying("TimerClick", nullptr))
		audio->resume("TimerClick", nullptr);

	if (audio->isPlaying("Fire", this))
		audio->resume("Fire", this);

	for (UniqueQueue<Trap>::iterator iter = mTriggeredTraps.begin(); iter != mTriggeredTraps.end(); iter++)
		iter->resume();

	for (UniqueQueue<Trap>::iterator iter = mUntriggeredTraps.begin(); iter != mUntriggeredTraps.end(); iter++)
		iter->resume();
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

				Trap& trap = mUntriggeredTraps.pop();
				trap.reset();

				mTriggeredTraps.push(trap);

				VectorF position = mapTile(trap)->rect().Center();

				AudioManager* audio = AudioManager::Get();
				if(audio->isPlaying("TimerClick", nullptr))
					audio->stop("TimerClick", nullptr);
				audio->play("SpikeTrapTrigger", tile, position);
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
				mTriggeredTraps.pop();

				VectorF position = mapTile(trap)->rect().Center();
				AudioManager::Get()->play("SpikeTrapReset", tile, position);
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


MapTile* TrapManager::mapTile(const Trap& trap) const
{
	Index trapIndex = trap.index();
	return mMap->tile(trapIndex);
}

void TrapManager::resolveCollision(Trap& trap)
{
	Effect* damageEffect = mEffectPool->getObject(EffectType::Damage);
	damageEffect->fill(trap.properties());

	mActor->addEffect(damageEffect);
	mActor->collider()->setGotHit(true);
	trap.setExhausted(true);
}
