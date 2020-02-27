#include "pch.h"
#include "PlayerPropertyBag.h"
#include "System/Files/StatReader.h"

#include "Characters/Attributes/Level.h"
#include "Characters/Attributes/Health.h"

void PlayerPropertyBag::readAttributes(std::string name)
{
	StatReader statReader;
	XMLValueMap map = statReader.getStats(FileManager::Config_Stats_Player, name);


	// Weapon
	const char* weaponName = map.getChar("Weapon");
	pWeapon.set(weaponName);


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
	pLevel.set(Level());

	// Health
	Health health(map.getFloat("Health"));
	pHealth.set(health);

	// Defence
	float defence = map.getFloat("Defence");
	pDefence.set(defence);

	// Attack Damage
	Damage attackDamage(map.getFloat("AttackDamage"));
	pAttackDmg.set(attackDamage);

	// Attack Speed
	double attackSpeed = map.getFloat("AttackSpeed");
	pAttackSpd.set(attackSpeed);

	// Movement Speed
	float movementSpeed = map.getFloat("MovementSpeed");
	pMovementSpd.set(movementSpeed);


	// -- Attribute Level up rates -- //
	
	// Health up rate
	Health healthUp(map.getFloat("HealthUpRate"));
	pHealthUp.set(healthUp);

	// Defence up rate
	float defenceUp = map.getFloat("DefenceUpRate");
	pDefenceUp.set(defenceUp);

	// Attack Damage up rate
	Damage attackDmgUp(map.getFloat("AttackDamageUpRate"));
	pAttackDmgUp.set(attackDmgUp);

	// Attack Speed up rate
	float attackSpdUp = map.getFloat("AttackSpeedUpRate");
	pAttackSpdUp.set(attackSpdUp);

	// Movement Speed up rate
	float movementSpdUp = map.getFloat("MovementSpeedUpRate");
	pMovementSpdUp.set(movementSpdUp);
}
