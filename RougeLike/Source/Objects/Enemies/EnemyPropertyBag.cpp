#include "pch.h"
#include "EnemyPropertyBag.h"
#include "System/Files/StatReader.h"


void EnemyPropertyBag::readAttributes(const std::string& name)
{
	StatReader statReader;
	XMLValueMap map = statReader.getStats(FileManager::Config_Enemies, name);

	// --- Physics --- //

	// Force
	float force = map.getFloat("Force");
	pForce.set(force);

	// Max velocity
	float maxVelocity = map.getFloat("MaxVelocity");
	pMaxVelocity.set(maxVelocity);

	// Drag force
	float dragFactor = map.getFloat("DragFactor");
	pDragFactor.set(dragFactor);

	// Mass
	float mass = map.getFloat("Mass");
	pMass.set(mass);


	// Health
	Health health(map.getInt("Health"));
	pHealth.set(health);

	// Movement
	int movementSpeed = map.getInt("MovementSpeed");
	pMovementSpeed.set(movementSpeed);

	// Damage
	Damage damage(map.getInt("AttackDamage"));
	pDamage.set(damage);

	// Tackle Speed
	float tackleSpeed = map.getFloat("TackleSpeed");
	pTackleSpeed.set(tackleSpeed);

	// Tackle Distance
	float tackleDistance = map.getFloat("TackleDistance");
	pTackleDistance.set(tackleDistance);

	// Tackle Charge Time
	float tackleChargeTime = map.getFloat("TackleChargeTime");
	pTackleChargeTime.set(tackleChargeTime);

	// Knockback Distance
	float knockbackDistance = map.getFloat("KnockbackDistance");
	pKnockbackDistance.set(knockbackDistance);

	// Sight Range
	float sightRange = map.getFloat("SightRange");
	pSightRange.set(sightRange);

	// Chase Range
	float chaseRange = map.getFloat("ChaseRange");
	pChaseRange.set(chaseRange);

	// Hurt time
	float hurtTime = map.getFloat("HurtTime");
	pHurtTime.set(hurtTime);

	// Attention Time
	float attentionTime = map.getFloat("AttentionTime");
	pAttentionTime.set(attentionTime);

	// Idle Time
	float idleTime = map.getFloat("IdleTime");
	pIdleTime.set(idleTime);

	// Score
	int score = map.getInt("Score");
	pScore.set(score);

	// Experience
	int exp = map.getInt("Experience");
	pExp.set(exp);
}