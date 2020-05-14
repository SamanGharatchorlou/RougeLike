#include "pch.h"
#include "SlowEffect.h"

#include "Objects/Actors/Actor.h"
#include "Objects/Properties/Property.h"


void SlowEffect::init()
{
	std::string slowingParameters[4]{ "Force", "MaxVelocity", "AttackSpeed", "TackleMovementSpeed" };

	for (int i = 0; i < 4; i++)
	{
		Property* property = mActor->getProperty(slowingParameters[i]);

		if (property)
			property->setValue(property->value() * mSlowFactor);
	}

	mActor->physics()->init(mActor->getPropertyValue("Force"), mActor->getPropertyValue("MaxVelocity"));
	mActor->animator().setSpeedFactor(mSlowFactor);
}


void SlowEffect::slowUpdate(float dt)
{

}


void SlowEffect::exit()
{
	std::string slowingParameters[4]{ "Force", "MaxVelocity", "AttackSpeed", "TacleMovementSpeed" };

	for (int i = 0; i < 4; i++)
	{
		Property* property = mActor->getProperty(slowingParameters[i]);

		if (property)
			property->setValue(property->value() / mSlowFactor);
	}

	mActor->physics()->init(mActor->getPropertyValue(slowingParameters[0]), mActor->getPropertyValue(slowingParameters[1]));
}