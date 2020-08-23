#include "pch.h"
#include "HealEffect.h"

#include "Actors/Actor.h"


void HealEffect::init()
{
	Health* health = static_cast<Health*>(mReceiver->getAttribute(AttributeType::Health));
	health->increase(mHeal);

	HealthChangedEvent* eventPtr = new HealthChangedEvent();
	EventPacket event(eventPtr);
	mReceiver->events().push(event);

	endEffect();
}


void HealEffect::fill(const PropertyMap& properties)
{
	mHeal = Health(properties.at(PropertyType::Health));
}


void HealEffect::exit()
{
	mHeal = Health(0.0f);
}