#include "pch.h"
#include "StunEffect.h"

#include "Actors/Enemies/Enemy.h"


void StunEffect::fill(const PropertyMap& valueBag)
{
	setProperty(PropertyType::StunTime, mTime, valueBag);
}

void StunEffect::init()
{
	Enemy* enemy = static_cast<Enemy*>(mReceiver);
	if (enemy)
		enemy->stun(mTime);

	endEffect();
}

void StunEffect::exit()
{
	mTime = 0.0f;
}