#include "pch.h"
#include "PlayerPropertyBag.h"
#include "System/Files/StatReader.h"

#include "Characters/Attributes/Level.h"
#include "Characters/Attributes/Health.h"

void PlayerPropertyBag::readAttributes(std::string name)
{
	StatReader statReader;
	XMLValueMap map = statReader.getStats(FileManager::Config_Stats_Player, name);


	// -- Movement -- //

	// Force
	float force = map.getFloat("Force");
	pForce.set(force);

	// Max velocity
	float maxVelocity = map.getFloat("MaxVelocity");
	pMaxVelocity.set(maxVelocity);

	// Drag force
	float dragFactor = map.getFloat("DragFactor");
	pDragFactor.set(dragFactor);


	// -- Attributes -- //

	// Level
	Level level(map.getInt("Level"));
	pLevel.set(level);

	// Health
	Health health(map.getFloat("Health"));
	pHealth.set(health);

	// Defence
	float defence = map.getFloat("Defence");
	pDefence.set(defence);

	// Defence
	float attackDamage = map.getFloat("AttackDamage");
	pAttackDmg.set(attackDamage);

	// Defence
	float attackSpeed = map.getFloat("AttackSpeed");
	pAttackSpd.set(attackSpeed);

	// Defence
	float movementSpeed = map.getFloat("MovementSpeed");
	pMovementSpd.set(movementSpeed);
}
