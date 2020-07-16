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