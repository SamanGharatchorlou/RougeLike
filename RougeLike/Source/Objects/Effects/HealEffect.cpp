#include "pch.h"
#include "HealEffect.h"

#include "Objects/Actors/Actor.h"
#include "Objects/Properties/Property.h"
#include "Objects/Attributes/Health.h"


void HealEffect::init()
{
	Health* health = static_cast<Health*>(mReceiver->getProperty("Health"));
	health->increase(mHeal);
	endEffect();
}
