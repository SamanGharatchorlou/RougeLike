#include "pch.h"
#include "BilnkAbility.h"

#include "Actors/Player/Player.h"
#include "Objects/Effects/EffectTypes/BlinkEffect.h"
#include "Objects/Pools/EffectPool.h"


//void BlinkAbility::activateAt(VectorF position, EffectPool* effectPool)
//{
//	mProperties.addXYPosition(position);
//
//	mRect.SetSize(realiseSize(mAnimator.frameSize(), mProperties[PropertyType::MaxSize]));
//	mRect.SetCenter(position);
//
//	mAnimator.startAnimation(Action::Active);
//
//	applyEffects(effectPool);
//
//	controlPlayer(true);
//}
//
//
//void BlinkAbility::slowUpdate(float dt)
//{
//	if (mReachedTarget)
//	{
//		VectorF center = mRect.Center();
//		mRect.SetSize(mRect.Size() * 0.96);
//		mRect.SetCenter(center);
//
//		if (mRect.Size().x < 1.0f)
//		{
//			mCompleted = true;
//			mAnimator.stop();
//		}
//	}
//
//	if (!mReachedTarget && distanceSquared(mCaster->position(), mRect.Center()) < mRect.Width())
//	{
//		mReachedTarget = true;
//		controlPlayer(false);
//	}
//
//	mAnimator.slowUpdate(dt);
//}
//
//
//void BlinkAbility::applyEffects(EffectPool* pool)
//{
//	applyEffect(EffectType::Blink, mCaster, pool);
//}
//
//
//void BlinkAbility::exit()
//{
//	mReachedTarget = false;
//}
//
//
//void BlinkAbility::controlPlayer(bool takeControl)
//{
//	Player* player = static_cast<Player*>(mCaster);
//
//	player->setVisibility(!takeControl);
//	player->overrideControl(takeControl);
//}