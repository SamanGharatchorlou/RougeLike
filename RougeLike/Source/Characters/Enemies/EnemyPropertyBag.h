#pragma once
#include "Characters/Attributes/Property.h"
#include "Characters/Attributes/Health.h"
#include "Characters/Attributes/Damage.h"

class EnemyPropertyBag
{
public:
	void readAttributes(std::string name);

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
	ExperienceProperty pExp;
};


