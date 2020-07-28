#include "pch.h"
#include "DamageEffect.h"

#include "Actors/Actor.h"
#include "Objects/Properties/PropertyBag.h"


void DamageEffect::init()
{
	if (mReceiver->hasProperty("Armor"))
	{
		Armor* armor = static_cast<Armor*>(mReceiver->getProperty("Armor"));
		if (armor->hasArmor())
		{
			armor->reduce(mDamage);
		}
		else
		{
			Health* hp = static_cast<Health*>(mReceiver->getProperty("Health"));
			hp->reduce(mDamage);
		}
	}
	else
	{
		Health* hp = static_cast<Health*>(mReceiver->getProperty("Health"));
		hp->reduce(mDamage);
	}

	mReceiver->collider()->setGotHit(true);
	endEffect();
}


void DamageEffect::clearData()
{
	clearBaseData();
	mDamage = Damage(0.0f);
}

void DamageEffect::fill(ValueMap& valueMap)
{
	std::string damage = "Damage";
	if (valueMap.count(damage))
		mDamage = Damage(valueMap[damage]);
	else
		DebugPrint(Warning, "Value map has no %s effect node\n", damage.c_str());
}