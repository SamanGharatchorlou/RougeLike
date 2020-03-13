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
	MovementSpeedProperty pMovementSpeed;

	// Attack
	AttackDamageProperty pDamage;
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

	// Idle
	IdleTimeProperty pIdleTime;

	// Score
	ScoreProperty pScore;
	ExperienceProperty pExp;
};


