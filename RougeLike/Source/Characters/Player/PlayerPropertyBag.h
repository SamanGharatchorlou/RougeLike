#pragma once
#include "Characters/Attributes/Property.h"


class PlayerPropertyBag
{
public:
	PlayerPropertyBag() { }
	void readAttributes(std::string name);

public:

	// Movement
	ForceProperty pForce;
	MaxVelocityProperty pMaxVelocity;
	DragFactorProperty pDragFactor;

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
};


