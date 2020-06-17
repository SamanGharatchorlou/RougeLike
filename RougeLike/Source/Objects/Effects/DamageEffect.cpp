#include "pch.h"
#include "DamageEffect.h"

#include "Objects/Actors/Actor.h"

void DamageEffect::fillData(const Actor* distributer)
{
	if (hasProperty(distributer, "Damage"))
		mDamage = distributer->getPropertyValue("Damage");
}


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
