#include "pch.h"
#include "HealEffect.h"

#include "Actors/Actor.h"


void HealEffect::init()
{
	Health* health = static_cast<Health*>(mReceiver->getAttribute(AttributeType::Health));
	health->increase(mHeal);
	endEffect();
}


void HealEffect::fill(PropertyBag& properties)
{
	mHeal = Health(properties.get(PropertyType::Health));
}


void HealEffect::exit()
{
	mHeal = Health(0.0f);
}