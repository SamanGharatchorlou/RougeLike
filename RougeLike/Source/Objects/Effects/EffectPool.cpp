#include "pch.h"
#include "EffectPool.h"

//#include "Game/GameData.h"
//#include "Animations/AnimationReader.h"

#include "EffectTypes/DamageEffect.h"
#include "EffectTypes/DisplacementEffect.h"
#include "EffectTypes/KnockbackStunEffect.h"
#include "EffectTypes/ArmorEffect.h"
#include "EffectTypes/BlinkEffect.h"
#include "EffectTypes/HealEffect.h"
#include "EffectTypes/StunEffect.h"


EffectPool::EffectPool()
{
	mPoolSizes[EffectType::None] = 0;
	mPool[EffectType::None] = std::queue<Effect*>();
}

void EffectPool::load()
{
	int defaultPoolSize = 10;

	for (EffectType type = EffectType::None + 1; type < EffectType::Count; type = type + 1)
	{
		std::queue<Effect*> effects;

		for (int i = 0; i < defaultPoolSize; i++)
		{
			effects.push(getNewEffect(type));
		}

		mPool[type] = effects;
		mPoolSizes[type] = defaultPoolSize;

#if _DEBUG
		mTrackerPool[type] = effects;
#endif
	}
}


void EffectPool::slowUpdate()
{
	// Top up pool, only new 1 effect per frame
	for (EffectType type = EffectType::None + 1; type < EffectType::Count; type = type + 1)
	{
		if (mPool[type].size() < mPoolSizes[type])
		{
			mPool[type].push(getNewEffect(type));
		}
	}
}


// TODO: set some kind of requirment so effects can only be created here, i.e. requires a null void* pointer or something?
Effect* EffectPool::getNewEffect(EffectType type)
{
	Effect* effect = nullptr;

	if (type == EffectType::Damage)
	{
		effect = new DamageEffect;
	}
	else if (type == EffectType::Displacement)
	{
		effect = new DisplacementEffect;
	}
	else if (type == EffectType::KnockbackStun)
	{
		Effect* additionalEffect = getEffect(EffectType::Stun);
		StunEffect* stunEffect = static_cast<StunEffect*>(additionalEffect);

		effect = new KnockbackStunEffect(stunEffect);
	}
	else if (type == EffectType::Armor)
	{
		effect = new ArmorEffect;
	}
	else if (type == EffectType::Blink)
	{
		effect = new BlinkEffect;
	}
	else if (type == EffectType::Heal)
	{
		effect = new HealEffect;
	}
	//else if (type == EffectType::Stun)
	//{
	//	XMLParser parser;
	//	parser.parseXML(FileManager::Get()->findFile(FileManager::Config_Abilities, "Stun"));

	//	AnimationReader reader(mGameData->textureManager, parser);
	//	Animator animator;
	//	reader.initAnimator(animator);

	//	effect = new StunEffect(animator);
	//}

#if _DEBUG
	mTrackerPool[type].push(effect);
#endif

	if(!effect)
		DebugPrint(Warning, "Could not find effect of type %d\n", (int)type);

	return effect;
}


Effect* EffectPool::getEffect(EffectType type)
{
	if (mPool[type].size() > 0)
	{
		Effect* effect = mPool[type].front();
		mPool[type].pop();
		return effect;
	}
	else
	{
		mPool[type].push(getNewEffect(type));
		mPoolSizes[type] = mPoolSizes[type] + 1;
		return getEffect(type);
	}
}


void EffectPool::returnEffect(Effect* effect)
{
	EffectType type = effect->type();
	effect->clearData();

	mPool[type].push(effect);
}
