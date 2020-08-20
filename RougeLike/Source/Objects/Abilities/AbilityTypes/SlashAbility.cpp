#include "pch.h"
#include "SlashAbility.h"

#include "Actors/Actor.h"

#include "Objects/Effects/EffectTypes/Effect.h"
#include "Objects/Pools/EffectPool.h"



void SlashAbility::activateAt(VectorF position, EffectPool* effectPool)
{
	mRect.SetLeftCenter(mCaster->rect().RightCenter());
	mAnimator.startAnimation(Action::Active);

	slashOnce();
}




void SlashAbility::activateOn(Actor* actor, EffectPool* effectPool)
{
	applyEffects(actor, effectPool);
}


void SlashAbility::applyEffects(Actor* actor, EffectPool* effectPool)
{
	if (mSlashCount == 1)
	{
		Effect* displacement = effectPool->getObject(EffectType::Displacement);
		displacement->fill(mProperties);
		actor->addEffect(displacement);
	}

	Effect* damage = effectPool->getObject(EffectType::Damage);
	damage->fill(mProperties);

	actor->addEffect(damage);
}


void SlashAbility::slowUpdate(float dt)
{
	mRect.SetLeftCenter(mCaster->rect().RightCenter());
	mAnimator.slowUpdate(dt);


	if (mAnimator.loops() == 1)
	{
		slashOnce();
	}
	else if (mAnimator.loops() > 1)
	{
		mAnimator.stop();
		mCompleted = true;
	}
}


void SlashAbility::slashOnce()
{
	if (mSlashCount < 2)
	{
		if (mSlashCount == 1)
		{
			mRect.SetSize(mRect.Size() * 2.0f);
		}

		mSlashCount++;
		sendActivateOnRequest();
	}
}


void SlashAbility::exit()
{
	mRect.SetSize(mRect.Size() / 2.0f);
	mSlashCount = 0;

	Ability::exit();
}