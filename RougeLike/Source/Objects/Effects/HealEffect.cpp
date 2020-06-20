#include "pch.h"
#include "HealEffect.h"

#include "Objects/Actors/Actor.h"


void HealEffect::init()
{
	Health* health = static_cast<Health*>(mReceiver->getProperty("Health"));
	health->increase(mHeal);
	endEffect();
}


void HealEffect::fillData(const EffectPropertyBag* properties)
{
	if (properties->contains("Heal"))
	{
		Property* property = properties->get("Heal");
		mHeal = *static_cast<Health*>(property);
	}
}

void HealEffect::clearData()
{
	clearBaseData();
	mHeal = Health(0.0f);
}