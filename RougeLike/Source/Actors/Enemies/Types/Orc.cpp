#include "pch.h"
#include "Orc.h"

#include "Objects/Abilities/AbilityClasses/AbilityStates.h"
#include "Objects/Abilities/AbilityClasses/Ability.h"

Orc::Orc() { }


void Orc::meleeAttack()
{
	replaceState(EnemyState::Charge);
}


bool Orc::canAttck() const
{
	VectorF startPosition = position();
	VectorF targetPosition = target()->position();
	VectorF direction = (targetPosition - startPosition).normalise();
	VectorF velocity = direction * getAttributeValue(AttributeType::TackleMovementSpeed);

	float maxDistance = getAttributeValue(AttributeType::TackleDistance);
	float targetDistance = distance(startPosition, targetPosition);

	// if close, just attack anyway!
	if (targetDistance < 70.0f)
		return true;

	// If we attack will we hit into a wall?
	RectF phantonRect = scaledRect();
	WallCollisionTracker collisions;
	collisions.setCollider(&mCollider);

	// Im a stupid orc and will only check if there's a wall between me and my target... and not behind my target
	float travelDistance = std::min(targetDistance, maxDistance);
	float delta = 0.1f;

	while (travelDistance >= 30.0f)
	{
		velocity = collisions.allowedVelocity(currentMap(), velocity, delta);

		if (velocity.x == 0.0f || velocity.y == 0.0f)
			return false;

		VectorF movement = velocity * delta;
		travelDistance -= distance(VectorF(), movement);
		phantonRect = phantonRect.Translate(movement);
	}

	return true;
}


bool Orc::isAttacking() const
{
	return state() == EnemyState::Charge || state() == EnemyState::PreAttack;
}