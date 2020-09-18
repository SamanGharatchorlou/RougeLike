#include "pch.h"
#include "HealAbility.h"

#include "Actors/Actor.h"
#include "Objects/Pools/EffectPool.h"

#include "Audio/AudioManager.h"



void HealAbility::slowUpdate(float dt)
{
	mAnimator.slowUpdate(dt);
	mRect.SetBotCenter(mCaster->rect().BotCenter() - VectorF(0.0f, 10.0f));

	// Completed one animation loop
	if (mAnimator.loops() > 0)
	{
		mAnimator.stop();
		mCompleted = true;
	}
}


void HealAbility::applyEffects(Actor* actor, EffectPool* effectPool)
{
	applyEffect(EffectType::Heal, actor, effectPool);
}



void HealAbility::playAudio() 
{
	AudioManager::Get()->pushEvent(AudioEvent(AudioEvent::Play, "HealAbility", mCaster));
}