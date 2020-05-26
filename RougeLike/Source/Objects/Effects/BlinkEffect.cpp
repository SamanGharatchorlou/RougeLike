#include "pch.h"
#include "BlinkEffect.h"

#include "Objects/Actors/Actor.h"
#include "Graphics/Texture.h"

#include "Objects/Actors/Player/Player.h"


void BlinkEffect::init()
{
	//mActor->animator().getSpriteTexture()->setAlpha(alphaMin);
	mActor->setVisibility(false);

	// NOTE: This bit only works for a player specific, if you ever need to do this to another object
	// then change this bit, move it into BlinkAbility perhaps?
	Player* player = static_cast<Player*>(mActor);
	player->userHasControl(false);
}


void BlinkEffect::slowUpdate(float dt)
{
	VectorF direction = (mTarget - mActor->position()).normalise();
	float speed = 2500.0f;

	float movementStep = distanceSquared(VectorF(), direction * speed * dt);
	float distanceToTarget = distanceSquared(mActor->position(), mTarget);

	if (distanceToTarget < movementStep)
	{
		VectorF finalStep = mTarget - mActor->position();
		mActor->physics()->move(finalStep / dt, dt);
		endEffect();
	}
	else
		mActor->physics()->move(direction * speed, dt);
}


void BlinkEffect::exit()
{
	//mActor->animator().getSpriteTexture()->setAlpha(alphaMax);
	mActor->setVisibility(true);

	// NOTE: This bit only works for a player specific, if you ever need to do this to another object
	// then change this bit, move it into BlinkAbility perhaps?
	Player* player = static_cast<Player*>(mActor);
	player->userHasControl(true);
}