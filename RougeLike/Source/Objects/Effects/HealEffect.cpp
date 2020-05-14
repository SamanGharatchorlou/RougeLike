#include "pch.h"
#include "HealEffect.h"

#include "Objects/Actors/Actor.h"
#include "Objects/Properties/Property.h"
#include "Objects/Attributes/Health.h"


void HealEffect::init()
{
	Property* property = mActor->getProperty("Health");
	Health* health = static_cast<Health*>(property);

	health->increase(mHeal);
	endEffect();
}
