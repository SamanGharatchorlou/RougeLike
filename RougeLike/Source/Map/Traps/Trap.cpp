#include "pch.h"
#include "Trap.h"

#include "Objects/Effects/EffectPool.h"
#include "Objects/Effects/EffectTypes/Effect.h"

#include "Map/Map.h"
#include "Map/Tiles/MapTile.h"
#include "Map/Tiles/TileTypes.h"


Trap::Trap(MapTile* tile) : mTile(tile), mCollider(&tile->rectRef()), exhausted(false)
{ 

}

void Trap::set()
{
	mTimer.start();
	mCollider.reset();
}


// Spike trap
void SpikeTrap::fillData(const DecorMap& data)
{
	const StringMap& attributes = data.at(DecorType::Spikes);

	info[PropertyType::Damage] = attributes.getFloat("damage");
	mTriggerTime = attributes.getFloat("triggerTime");
	mRecoveryTime = attributes.getFloat("recoveryTime");
}


void SpikeTrap::init(EffectPool* effects)
{
	if (!mCollider.hasEffects())
	{
		Effect* effect = effects->getObject(EffectType::Damage);
		effect->fill(info);
		mCollider.addEffect(effect);
	}
}


void SpikeTrap::trigger()
{
	Animator& animator = mTile->animation(0);
	animator.getAnimation(0).nextFrame();

	exhausted = true;

	mTimer.restart();
	mCollider.reset();
}


void SpikeTrap::reset()
{
	Animator& animator = mTile->animation(0);
	animator.getAnimation(0).nextFrame();

	exhausted = false;
	mTimer.stop();
}


// Fire grating
void FireGratingTrap::fillData(const DecorMap& data)
{
	const StringMap& attributes = data.at(DecorType::Spikes);

	info[PropertyType::Damage] = attributes.getFloat("damage");
	mTriggerTime = attributes.getFloat("triggerTime");
	mRecoveryTime = attributes.getFloat("recoveryTime");
}

void FireGratingTrap::init(EffectPool* effects)
{
	if (!mCollider.hasEffects())
	{
		Effect* effect = effects->getObject(EffectType::Damage);
		effect->fill(info);
		mCollider.addEffect(effect);
	}
}

// replace this with the init function somehow??
void FireGratingTrap::update(EffectPool* effects)
{
	Animator& animator = mTile->animation(0);
	if (animator.loops() > mLoops)
	{
		mLoops = animator.loops();
		init(effects);
	}
}


void FireGratingTrap::trigger()
{
	if (mActive)
	{
		Animator& animator = mTile->animation(0);
		animator.startAnimation(Action::Active);

		exhausted = true;

		mTimer.restart();
		mCollider.reset();
	}
}

void FireGratingTrap::reset()
{
	Animator& animator = mTile->animation(0);
	//animator.stop();

	exhausted = false;
	mTimer.stop();
}

// Spik trap trigger
void FireTrapTrigger::fillData(const DecorMap& data)
{
	const StringMap& attributes = data.at(DecorType::Trigger);

	mTriggerTime = attributes.getFloat("triggerTime");
	mRecoveryTime = attributes.getFloat("recoveryTime");
}

void FireTrapTrigger::trigger()
{
	Animator& animator = mTile->animation(0);
	animator.getAnimation(0).nextFrame();

	exhausted = true;

	mTimer.restart();
	mCollider.reset();

	for (int i = 0; i < mTraps.size(); i++)
	{
		Trap* trap = mTraps[i];
		if (trap->tile()->has(DecorType::Grating))
		{
			FireGratingTrap* fireTrap = static_cast<FireGratingTrap*>(trap);
			fireTrap->setActive(true);
			fireTrap->set();
		}
	}

	printf("triggered\n");

}

void FireTrapTrigger::reset()
{
	mTimer.stop();
}