#include "pch.h"
#include "DamageEffect.h"

#include "Actors/Actor.h"
#include "Objects/Properties/PropertyBag.h"

#include "Objects/Properties/PropertyTypes/Armor.h"
#include "Objects/Properties/PropertyTypes/Health.h"

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


void DamageEffect::fill(const ValueBag& valueBag)
{
	setValue("Damage", mDamage, valueBag);
}

void DamageEffect::exit()
{
	mDamage = Damage(0.0f);
}