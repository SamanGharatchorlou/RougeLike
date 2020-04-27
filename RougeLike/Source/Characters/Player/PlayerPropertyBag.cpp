#include "pch.h"
#include "PlayerPropertyBag.h"
#include "System/Files/StatReader.h"

#include "Characters/Attributes/Level.h"
#include "Characters/Attributes/Health.h"

void PlayerPropertyBag::readAttributes(const std::string& name)
{
	StatReader statReader;
	XMLValueMap map = statReader.getStats(FileManager::Config_Stats_Player, name);


	// Weapon
	const char* weaponName = map.getChar("Weapon");
	pWeapon.set(weaponName);


	// -- Physics -- //

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


	// -- Attributes -- //

	// Level
	pLevel.set(Level());

	// Health
	Health health(map.getInt("Health"));
	pHealth.set(health);

	// Defence
	int defence = map.getInt("Defence");
	pDefence.set(defence);

	// Attack Damage
	Damage attackDamage(map.getInt("AttackDamage"));
	pAttackDmg.set(attackDamage);

	// Attack Speed
	float attackSpeed = map.getFloat("AttackSpeed");
	pAttackSpd.set(attackSpeed);

	// Movement Speed
	int movementSpeed = map.getInt("MovementSpeed");
	pMovementSpd.set(movementSpeed);


	// -- Attribute Level up rates -- //
	
	// Health up rate
	Health healthUp(map.getInt("HealthUpRate"));
	pHealthUp.set(healthUp);

	// Defence up rate
	int defenceUp = map.getInt("DefenceUpRate");
	pDefenceUp.set(defenceUp);

	// Attack Damage up rate
	Damage attackDmgUp(map.getInt("AttackDamageUpRate"));
	pAttackDmgUp.set(attackDmgUp);

	// Attack Speed up rate
	float attackSpdUp = map.getFloat("AttackSpeedUpRate");
	pAttackSpdUp.set(attackSpdUp);

	// Movement Speed up rate
	int movementSpdUp = map.getInt("MovementSpeedUpRate");
	pMovementSpdUp.set(movementSpdUp);
}
