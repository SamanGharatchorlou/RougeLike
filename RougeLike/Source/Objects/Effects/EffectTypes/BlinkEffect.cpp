#include "pch.h"
#include "BlinkEffect.h"

#include "Objects/Actors/Actor.h"
#include "Graphics/Texture.h"

#include "Objects/Actors/Player/Player.h"


void BlinkEffect::init()
{
	mReceiver->setVisibility(false);

	// NOTE: This bit only works for a player specific, if you ever need to do this to another object
	// then change this bit, move it into BlinkAbility perhaps?
	Player* player = static_cast<Player*>(mReceiver);
	player->overrideControl(true);
}


void BlinkEffect::slowUpdate(float dt)
{
	VectorF direction = (mTarget - mReceiver->position()).normalise();
	float speed = 2500.0f;

	float movementStep = distanceSquared(VectorF(), direction * speed * dt);
	float distanceToTarget = distanceSquared(mReceiver->position(), mTarget);

	if (distanceToTarget < movementStep)
	{
		VectorF finalStep = mTarget - mReceiver->position();
		mReceiver->physics()->move(finalStep / dt, dt);
		endEffect();
	}
	else
		mReceiver->physics()->move(direction * speed, dt);
}


void BlinkEffect::exit()
{
	mReceiver->setVisibility(true);

	// NOTE: This bit only works for a player specific, if you ever need to do this to another object
	// then change this bit, move it into BlinkAbility perhaps?
	Player* player = static_cast<Player*>(mReceiver);
	player->overrideControl(false);
}


void BlinkEffect::clearData()
{
	clearBaseData();
	mTarget = VectorF();
}
