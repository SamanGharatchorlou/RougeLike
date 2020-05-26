#include "pch.h"
#include "BlinkEffect.h"

#include "Objects/Actors/Actor.h"
#include "Graphics/Texture.h"

#include "Objects/Actors/Player/Player.h"

void BlinkEffect::init()
{
	mActor->animator().getSpriteTexture()->setAlpha(alphaMin);

	// NOTE: This bit only works for a player specific, if you ever need to do this to another object
	// then change this bit, move it into BlinkAbility perhaps?
	Player* player = static_cast<Player*>(mActor);
	player->checkWallCollisions(false);
}


void BlinkEffect::slowUpdate(float dt)
{
	VectorF direction = mTarget - mActor->position();
	float speed = 5.0f;

	mActor->physics()->move(direction * speed, dt);

	if (distanceSquared(mActor->position(), mTarget) < 100.0f)
	{
		endEffect();
	}
}


void BlinkEffect::exit()
{
	mActor->animator().getSpriteTexture()->setAlpha(alphaMax);

	// NOTE: This bit only works for a player specific, if you ever need to do this to another object
	// then change this bit, move it into BlinkAbility perhaps?
	Player* player = static_cast<Player*>(mActor);
	player->checkWallCollisions(true);
}