#include "pch.h"
#include "ChargeAbility.h"

#include "Objects/Effects/EffectPool.h"
#include "Objects/Effects/EffectTypes/DisplacementEffect.h"
#include "Objects/Actors/Player/Player.h"

#include "Map/Map.h"

// TEMP
#include "Debug/DebugDraw.h"


void ChargeAbility::fillValues(ValueMap& values)
{
	mChargeForce = std::stof(values["ChargeForce"]);
	mChargeDistance = std::stof(values["ChargeDistance"]);

	// Knockback data
	mDamage = Damage(std::stof(values["Damage"]));
	mKnockbackForce = std::stof(values["KnockbackForce"]);
	mKnockbackDistance = std::stof(values["KnockbackDistance"]);
}


void ChargeAbility::activateAt(VectorF position, EffectPool* effectPool)
{
	mDistanceTravelled = 0.0f;

	VectorF direction = (mCaster->position() - position).normalise();
	mChargeSource = mCaster->position() + direction; // any random point in this particular direction

	Player* player = static_cast<Player*>(mCaster);
	player->enableBodyCollisions(true);

	start = player->position();
	end = start - ( direction * mChargeDistance);
}


void ChargeAbility::activateOn(Actor* target, EffectPool* effectPool)
{
	if (mHitList.count(target) == 0)
	{
		applyEffects(target, effectPool);
		mHitList.insert(target);
		printf("applying effect onto actor\n");
	}
}




void ChargeAbility::fastUpdate(float dt)
{
	VectorF direction = (mCaster->position() - mChargeSource).normalise();
	VectorF velocity = direction * mChargeForce;
	float movementStep = std::sqrt(velocity.magnitudeSquared());

	if (mDistanceTravelled < mChargeDistance && canMove(mCaster, velocity, dt))
	{
		mCaster->physics()->move(velocity, dt);
		mDistanceTravelled += movementStep * dt;
	}
}


void ChargeAbility::slowUpdate(float dt)
{
	mRect = mCaster->collider()->scaledRect();

	if (mCaster->collider()->didHit())
	{
		sendActivateOnRequest();
	}
}

void ChargeAbility::render()
{
	if (mState == Selected)
	{
		renderRangeCircle();
	}

	// TODO: get the point on this line (the players charge line) which is clostest to the enemy position
	// this will allow you to provide a better 'source' value for the displacement effect.
	// maybe I can implement this on weapon attacks as well?
	//debugDrawLine(start, end, RenderColour::Red);
}


void ChargeAbility::exit()
{
	Player* player = static_cast<Player*>(mCaster);
	player->enableBodyCollisions(false);

	mDistanceTravelled = 0.0f;
	mHitList.clear();
}


//// --- Private Functions --- //
//bool ChargeAbility::canMove(VectorF velocity, float dt) const
//{
//	Index index;
//	const Map* map = mCaster->currentMap();
//	RectF rect = mCaster->scaledRect().Translate(velocity * dt);
//
//	index = map->index(rect.TopLeft());
//	if (!map->isValidIndex(index) || map->collisionType(index) == CollisionTile::Wall )
//		return false;
//
//	index = map->index(rect.TopRight());
//	if (!map->isValidIndex(index) || map->collisionType(index) == CollisionTile::Wall)
//		return false;
//
//	index = map->index(rect.BotRight());
//	if (!map->isValidIndex(index) || map->collisionType(index) == CollisionTile::Wall)
//		return false;
//
//	index = map->index(rect.BotLeft());
//	if (!map->isValidIndex(index) || map->collisionType(index) == CollisionTile::Wall)
//		return false;
//
//	return true;
//}



void ChargeAbility::applyEffects(Actor* actor, EffectPool* effectPool)
{
	Effect* displacement = effectPool->getEffect(EffectType::Displacement);
	DisplacementEffect* displacementEffect = static_cast<DisplacementEffect*>(displacement);
	displacementEffect->set(mCaster->position(), mKnockbackForce, mKnockbackDistance);
	actor->addEffect(displacement);
}