#include "pch.h"
#include "EffectPool.h"

#include "Objects/Effects/EffectTypes/Effect.h"
#include "Objects/Effects/EffectTypes/DamageEffect.h"
#include "Objects/Effects/EffectTypes/DisplacementEffect.h"
#include "Objects/Effects/EffectTypes/KnockbackStunEffect.h"
#include "Objects/Effects/EffectTypes/ArmorEffect.h"
#include "Objects/Effects/EffectTypes/BlinkEffect.h"
#include "Objects/Effects/EffectTypes/HealEffect.h"
#include "Objects/Effects/EffectTypes/StunEffect.h"


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
