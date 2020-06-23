#include "pch.h"
#include "ChargeAbility.h"

#include "Objects/Effects/EffectPool.h"
#include "Objects/Effects/DisplacementEffect.h"
#include "Objects/Actors/Player/Player.h"

#include "Map/Map.h"


void ChargeAbility::fillValues(ValueMap& values)
{
	// Charge data
	mMaxDimention = std::stof(values["MaxSize"]); // TODO: what would this be for?
	mCooldownTime = std::stof(values["Cooldown"]);
	mRange = std::stof(values["Range"]);

	mChargeForce = std::stof(values["ChargeForce"]);
	mChargeDistance = std::stof(values["ChargeDistance"]);

	// Knockback data
	mDamage = Damage(std::stof(values["Damage"]));
	mKnockbackForce = std::stof(values["KnockbackForce"]);
	mKnockbackDistance = std::stof(values["KnockbackDistance"]);
}


void ChargeAbility::activate(VectorF position)
{
	mDistanceTravelled = 0.0f;

	VectorF direction = (mCaster->position() - position).normalise();
	mChargeSource = mCaster->position() + direction; // any random point in this particular direction
}


void ChargeAbility::activate(Actor* target, EffectPool* effectPool)
{
	mEffectPool = effectPool;

	//Effect* effect = effectPool->getEffect(EffectType::Displacement);
	//DisplacementEffect* displacementEffect = static_cast<DisplacementEffect*>(effect);
	//displacementEffect->set(mChargeSource, mChargeForce, mChargeDistance);
	//mCaster->addEffect(effect);

	//mCaster->collider()->setIsActive(true);

	Player* player = static_cast<Player*>(mCaster);
	player->enableBodyCollisions(true);

	//mCaster->getProperty("Damage")->setValue(mDamage.value());
	//mCaster->getProperty("KnockbackForce")->setValue(mKnockbackForce);
	//mCaster->getProperty("KnockbackDistance")->setValue(mKnockbackDistance);
	beginCooldown();
}


void ChargeAbility::fastUpdate(float dt)
{
	VectorF direction = (mCaster->position() - mChargeSource).normalise();
	VectorF velocity = direction * mChargeForce;
	float movementStep = std::sqrt(velocity.magnitudeSquared());

	if (mDistanceTravelled >= mChargeDistance)
	{
		endAbility();
	}
	else if (canMove(velocity, dt))
	{
		mCaster->physics()->move(velocity, dt);
		mDistanceTravelled += movementStep * dt;
	}
}


void ChargeAbility::slowUpdate(float dt)
{
	if (hasCooledDown())
		endAbility();

	if (mCaster->collider()->didHit())
	{	
		Effect* effect = mEffectPool->getEffect(EffectType::Displacement);
		DisplacementEffect* displacementEffect = static_cast<DisplacementEffect*>(effect);
		displacementEffect->set(mCaster->position(), mKnockbackForce, mKnockbackDistance);

		mCaster->addEffect(effect);

	}
		printf("hit\n");
	// copy ability managers apply to multiple enemies thing
	// does this need to be a area ability then?
	// single point type, something between single enemy and self??
}


void ChargeAbility::exit()
{
	Player* player = static_cast<Player*>(mCaster);
	player->enableBodyCollisions(false);
}


// --- Private Functions --- //
bool ChargeAbility::canMove(VectorF velocity, float dt) const
{
	Index index;
	const Map* map = mCaster->currentMap();
	RectF rect = mCaster->scaledRect().Translate(velocity * dt);

	index = map->index(rect.TopLeft());
	if (!map->isValidIndex(index) || !map->floorCollisionTile(index))
		return false;

	index = map->index(rect.TopRight());
	if (!map->isValidIndex(index) || !map->floorCollisionTile(index))
		return false;

	index = map->index(rect.BotRight());
	if (!map->isValidIndex(index) || !map->floorCollisionTile(index))
		return false;

	index = map->index(rect.BotLeft());
	if (!map->isValidIndex(index) || !map->floorCollisionTile(index))
		return false;

	return true;
}