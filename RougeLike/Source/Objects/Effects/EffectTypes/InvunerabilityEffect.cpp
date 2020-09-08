#include "pch.h"
#include "InvunerabilityEffect.h"

#include "Actors/Actor.h"


void InvunerabilityEffect::init()
{
	Health* health = static_cast<Health*>(mReceiver->getAttribute(AttributeType::Health));
	health->setInvulnerablity(true);
	mTimer.start();
}

void InvunerabilityEffect::slowUpdate(float dt)
{
	if (mTimer.getSeconds() > mTime)
	{
		Health* health = static_cast<Health*>(mReceiver->getAttribute(AttributeType::Health));
		health->setInvulnerablity(false);
		endEffect();
	}
}

void InvunerabilityEffect::fill(const PropertyMap& properties)
{
	mTime = properties.at(PropertyType::Time);
}


void InvunerabilityEffect::exit()
{
	mTime = 0.0f;
	mTimer.stop();
}