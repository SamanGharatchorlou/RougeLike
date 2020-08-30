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

	if(mReact)
		mReceiver->collider()->setGotHit(true);

	HealthChangedEvent* eventPtr = new HealthChangedEvent();
	EventPacket event(eventPtr);
	mReceiver->events().push(event);

	printf("removing damage\n");

	endEffect();
}


void DamageEffect::fill(const PropertyMap& valueBag)
{
	setProperty(PropertyType::Damage, mDamage, valueBag);

	if (valueBag.contains(PropertyType::DamageReact))
	{
		float shouldReact = -1;
		setProperty(PropertyType::DamageReact, shouldReact, valueBag);
		mReact = (bool)shouldReact;
	}
}


void DamageEffect::exit()
{
	mDamage = Damage(0.0f);
}