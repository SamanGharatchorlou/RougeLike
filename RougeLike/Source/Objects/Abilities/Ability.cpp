#include "pch.h"
#include "Ability.h"

#include "Objects/Actors/Actor.h"

#include "Objects/Effects/SlowEffect.h"
#include "Objects/Effects/HealEffect.h"

#include "Objects/Properties/PropertyBag.h"
#include "Objects/Attributes/Health.h"


EventPacket Ability::popEvent()
{
	ASSERT(Error, mEvents.size() > 0, "Ability has no event when attempting to pop one.\n");
	EventPacket event = mEvents.front();
	mEvents.pop();
	return event;
}


void SlowAbility::activate(Actor* actor)
{
	SlowEffect* slowEffect = new SlowEffect(mSlowFactor);
	actor->addEffect(slowEffect);
	mActivated = true;
}


void HealAbility::activate(Actor* actor)
{
	HealEffect* healEffect = new HealEffect(mHeal);
	actor->addEffect(healEffect);

	mActivated = true;

	Health* hp = static_cast<Health*>(actor->getProperty("Health"));
	SetHealthBarEvent* dataPtr = new SetHealthBarEvent(*hp);
	mEvents.push(EventPacket(dataPtr));
}

