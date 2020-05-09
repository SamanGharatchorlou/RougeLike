#pragma once
// TODO: remoce these
#include "Objects/Attributes/Property.h"
#include "Objects/Attributes/Health.h"
#include "Objects/Attributes/Damage.h"

class EnemyPropertyBag
{
public:
	void readAttributes(const std::string& name);

public:
	// Physics
	ForceProperty pForce;
	MaxVelocityProperty pMaxVelocity;

	HealthProperty pHealth;

	// Attack
	AttackDamageProperty pDamage;
	TackleDistanceProperty pTackleDistance;
	TackleSpeedProperty pTackleSpeed;
	TackleChangeTimeProperty pTackleChargeTime;
	KnockbackDistanceProperty pKnockbackDistance;

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


