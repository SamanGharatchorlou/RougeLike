#include "pch.h"

#include "Ability.h"

#include "Objects/Actors/Actor.h"

#include "Objects/Effects/SlowEffect.h"
#include "Objects/Effects/HealEffect.h"



void SlowAbility::activate(Actor* actor)
{
	SlowEffect* slowEffect = new SlowEffect(mSlowFactor);
	actor->addEffect(slowEffect);
}


void HealAbility::activate(Actor* actor)
{
	HealEffect* healEffect = new HealEffect(mHeal);
	actor->addEffect(healEffect);
}