#include "pch.h"
#include "SlowAbility.h"

#include "Objects/Actors/Actor.h"
#include "Objects/Effects/SlowEffect.h"

void SlowAbility::activate(Actor* actor)
{
	SlowEffect* slowEffect = new SlowEffect(mSlowFactor);
	actor->addEffect(slowEffect);
}