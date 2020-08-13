#include "pch.h"
#include "FireGratingTrap.h"

#include "Objects/Pools/EffectPool.h"
#include "Objects/Effects/EffectTypes/Effect.h"

#include "Map/Tiles/MapTile.h"
#include "Map/Tiles/TileTypes.h"


void FireGratingTrap::fillData(const DecorMap& data)
{
	const StringMap& attributes = data.at(DecorType::Grating);

	info[PropertyType::Damage] = attributes.getFloat("damage");
	mTriggerTime = attributes.getFloat("triggerTime");
}

void FireGratingTrap::clear()
{
	mActive = false;
	mLoops = 0;
	Trap::clear();
}


// replace this with the init function somehow??
void FireGratingTrap::update(EffectPool* effects)
{
	Animator& animator = mTile->animation(0);
	if (animator.loops() > mLoops)
	{
		mLoops = animator.loops();

		if (didHit() && !mCollider.hasEffects())
		{
			Effect* effect = effects->getObject(EffectType::Damage);
			effect->fill(info);
			mCollider.addEffect(effect);
		}
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
