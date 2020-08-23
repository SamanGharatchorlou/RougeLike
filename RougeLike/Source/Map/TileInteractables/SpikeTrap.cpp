#include "pch.h"
#include "SpikeTrap.h"

#include "Objects/Pools/EffectPool.h"
#include "Objects/Effects/EffectTypes/Effect.h"

#include "Map/Tiles/MapTile.h"
#include "Map/Tiles/TileTypes.h"


void SpikeTrap::fillData(const DecorMap& data)
{
	const StringMap& attributes = data.at(DecorType::Spikes);

	info[PropertyType::Damage] = attributes.getFloat("damage");
	mTriggerTime = attributes.getFloat("triggerTime");
	mRecoveryTime = attributes.getFloat("recoveryTime");
}

void SpikeTrap::clear()
{
	mRecoveryTime = -1.0f;
	Trap::clear();
}



void SpikeTrap::update(EffectPool* effects)
{
	if (gotHit() && !mCollider.hasEffects())
	{
		Effect* effect = effects->getObject(EffectType::Damage);
		effect->fill(info);
		mCollider.addEffect(effect);
	}
}


void SpikeTrap::trigger()
{
	Animator& animator = mTile->animation(0);
	animator.activeAimation()->nextFrame();

	exhausted = true;

	mTimer.restart();
	mCollider.reset();
}


void SpikeTrap::reset()
{
	Animator& animator = mTile->animation(0);
	animator.activeAimation()->nextFrame();

	exhausted = false;
	mTimer.stop();
}
