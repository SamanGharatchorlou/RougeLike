#include "pch.h"
#include "EnemyPropertyBag.h"
#include "System/Files/StatReader.h"


void EnemyPropertyBag::readAttributes(std::string name)
{
	StatReader statReader;
	XMLValueMap map = statReader.getStats(FileManager::Config_Stats_Enemies, name);

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

	// Score
	int score = map.getInt("Score");
	pScore.set(score);

	// Experience
	int exp = map.getInt("Experience");
	pExp.set(exp);
}
