#include "pch.h"
#include "EffectPool.h"

#include "Objects/Effects/EffectTypes/DamageEffect.h"
#include "Objects/Effects/EffectTypes/DisplacementEffect.h"
#include "Objects/Effects/EffectTypes/KnockbackStunEffect.h"
#include "Objects/Effects/EffectTypes/ArmorEffect.h"
#include "Objects/Effects/EffectTypes/BlinkEffect.h"
#include "Objects/Effects/EffectTypes/HealEffect.h"
#include "Objects/Effects/EffectTypes/StunEffect.h"
#include "Objects/Effects/EffectTypes/InvunerabilityEffect.h"


Effect* EffectPool::createNewObjects(EffectType type, int count, int& outSize) const
{
	Effect* effect = nullptr;

	switch (type)
	{
	case EffectType::Heal:
		effect = new HealEffect[count];
		outSize = sizeof(HealEffect);
		break;

	case EffectType::Blink:
		effect = new BlinkEffect[count];
		outSize = sizeof(BlinkEffect);
		break;

	case EffectType::Damage:
		effect = new DamageEffect[count];
		outSize = sizeof(DamageEffect);
		break;

	case EffectType::Displacement:
		effect = new DisplacementEffect[count];
		outSize = sizeof(DisplacementEffect);
		break;

	case EffectType::Stun:
		effect = new StunEffect[count];
		outSize = sizeof(StunEffect);
		break;

	case EffectType::Armor:
		effect = new ArmorEffect[count];
		outSize = sizeof(ArmorEffect);
		break;

	case EffectType::KnockbackStun:
		effect = new KnockbackStunEffect[count];
		outSize = sizeof(KnockbackStunEffect);
		break;

	case EffectType::Invunerability:
		effect = new InvunerabilityEffect[count];
		outSize = sizeof(InvunerabilityEffect);
		break;

	case EffectType::Count:
	case EffectType::None:
	default:
		DebugPrint(Warning, "Invalid effect type %d, could not create %d objects\n", type, count);
		break;
	}

	return effect;
}
