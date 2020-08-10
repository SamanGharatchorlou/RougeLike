#include "pch.h"
#include "DamageEffect.h"

#include "Actors/Actor.h"

#include "Objects/Properties/Attributes/Armor.h"
#include "Objects/Properties/Attributes/Health.h"


void DamageEffect::init()
{
	if (mReceiver->hasAttribute(AttributeType::Armor))
	{
		Armor* armor = static_cast<Armor*>(mReceiver->getAttribute(AttributeType::Armor));
		if (armor->hasArmor())
		{
			armor->reduce(mDamage);
		}
		else
		{
			Health* hp = static_cast<Health*>(mReceiver->getAttribute(AttributeType::Health));
            hp->reduce(mDamage);
		}
	}
	else
	{
		Health* hp = static_cast<Health*>(mReceiver->getAttribute(AttributeType::Health));
		hp->reduce(mDamage);
	}

	mReceiver->collider()->setGotHit(true);
	endEffect();
}


void DamageEffect::fill(const PropertyBag& valueBag)
{
	setProperty(PropertyType::Damage, mDamage, valueBag);
}


void DamageEffect::exit()
{
	mDamage = Damage(0.0f);
}