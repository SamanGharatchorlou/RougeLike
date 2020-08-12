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



void SpikeTrapTrigger::fillData(const DecorMap& data)
{
	const StringMap& attributes = data.at(DecorType::Trigger);

	mTriggerTime = attributes.getFloat("triggerTime");
	mRecoveryTime = attributes.getFloat("recoveryTime");
}

void SpikeTrapTrigger::trigger()
{
	Animator& animator = mTile->animation(0);
	animator.getAnimation(0).nextFrame();

	exhausted = true;

	mTimer.restart();
	mCollider.reset();


	for (int y = 0; y < mMap->yCount(); y++)
	{
		for (int x = 0; x < mMap->xCount(); x++)
		{
			Index index(x, y);
			MapTile* tile = mMap->tile(index);

			if (tile->has(DecorType::Grating))
			{

			}
		}
	}

	printf("triggered\n");

}

void SpikeTrapTrigger::reset()
{
	mTimer.stop();
}