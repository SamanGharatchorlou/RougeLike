#include "pch.h"
#include "DamageEffect.h"

#include "Objects/Actors/Actor.h"
#include "Collisions/Collider.h"

#include "Objects/Properties/PropertyBag.h"
#include "Objects/Attributes/Health.h"

void DamageEffect::init()
{
	if (mActor->hasProperty("Armor"))
	{
		Armor* armor = static_cast<Armor*>(mActor->getProperty("Armor"));
		if (armor->hasArmor())
		{
			armor->reduce(mDamage);
		}
		else
		{
			Health* hp = static_cast<Health*>(mActor->getProperty("Health"));
			hp->reduce(mDamage);
		}
	}
	else
	{
		Health* hp = static_cast<Health*>(mActor->getProperty("Health"));
		hp->reduce(mDamage);
	}


	mActor->collider()->setGotHit(true);
	endEffect();
}
