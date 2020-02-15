#pragma once
#include "Characters/Attributes/Property.h"


class EnemyPropertyBag
{
public:
	HealthProperty pHealth;
	DamageProperty pDamage;
	MovementSpeedProperty pMovementSpeed;

	// Attack
	TackleDistanceProperty pTackleDistance;
	TackleSpeedProperty pTackleSpeed;
	TackleChangeTimeProperty pTackleChargeTime;

	// Run
	ChaseRangeProperty pChaseRange;

	// Attention
	SightRangeProperty pSightRange;
	AttentionTimeProperty pAttentionTime;

	// Hurt
	HurtTimeProperty pHurtTime;

	// Score
	ScoreProperty pScore;
};


