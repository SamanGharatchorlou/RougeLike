#pragma once
#include "Objects/Attributes/Property.h"


class PlayerPropertyBag
{
public:
	PlayerPropertyBag() { }
	void readAttributes(const std::string& name);

	void resetAttributes();

public:

	// Physics
	ForceProperty pForce;
	MaxVelocityProperty pMaxVelocity;
	DragFactorProperty pDragFactor;
	MassProperty pMass;

	// Attributes
	LevelProperty pLevel;

	HealthProperty pHealth;
	DefenceProperty pDefence;
	AttackDamageProperty pAttackDmg;
	AttackSpeedProperty pAttackSpd;
	MovementSpeedProperty pMovementSpd;

	// Attributes - Level up rates
	HealthUpRate pHealthUp;
	DefenceUpRate pDefenceUp;
	AttackDamageUpRate pAttackDmgUp;
	AttackSpeedUpRate pAttackSpdUp;
	MovementSpeedUpRate pMovementSpdUp;

	WeaponProperty pWeapon;


private:
	std::string attributeName;
};


