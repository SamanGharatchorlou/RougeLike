#include "pch.h"
#include "HealEffect.h"

#include "Actors/Actor.h"


void HealEffect::init()
{
	Health* health = static_cast<Health*>(mReceiver->getProperty("Health"));
	health->increase(mHeal);
	endEffect();
}


void HealEffect::clearData()
{
	clearBaseData();
	mHeal = Health(0.0f);
}

void HealEffect::fill(ValueMap& valueMap)
{
	std::string health = "Health";
	if (valueMap.count(health))
		mHeal = Health(valueMap[health]);
	else
		DebugPrint(Warning, "Value map has no %s effect node\n", health.c_str());
}