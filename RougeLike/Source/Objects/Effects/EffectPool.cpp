#include "pch.h"
#include "EffectPool.h"

//#include "Game/GameData.h"
//#include "Animations/AnimationReader.h"

#include "EffectTypes/Effect.h"

#include "EffectTypes/DamageEffect.h"
#include "EffectTypes/DisplacementEffect.h"
#include "EffectTypes/KnockbackStunEffect.h"
#include "EffectTypes/ArmorEffect.h"
#include "EffectTypes/BlinkEffect.h"
#include "EffectTypes/HealEffect.h"
#include "EffectTypes/StunEffect.h"



void EffectPool::load()
{
	int defaultPoolSize = 10;

	for (EffectType type = EffectType::None + 1; type < EffectType::Count; type = type + 1)
	{
		addNewObjects(type, defaultPoolSize);
	}
}

Effect* EffectPool::createNewObject(EffectType type) const
{
	Effect* effect = nullptr;

	switch (type)
	{
	case EffectType::Heal:
		effect = new HealEffect;
		break;
	case EffectType::Blink:
		effect = new BlinkEffect;
		break;
	case EffectType::Damage:
		effect = new DamageEffect;
		break;
	case EffectType::Displacement:
		effect = new DisplacementEffect;
		break;
	case EffectType::Armor:	
	case EffectType::Stun:
	case EffectType::KnockbackStun:
		DebugPrint(Log, "Effect of type %d is unimplemented\n", (int)type);
		break;
	case EffectType::Count:
	case EffectType::None:
	default:
		break;
	}

	return effect;
}



void EffectPool::returnObject(Effect* effect)
{
	// something needs to clear this object before... handled in effect handler?
	mPool[effect->type()].push(effect);
}
