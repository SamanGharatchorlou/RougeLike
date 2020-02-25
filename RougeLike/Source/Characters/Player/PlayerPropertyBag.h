#pragma once
#include "Characters/Attributes/Property.h"

class PlayerPropertyBag
{
public:
	PlayerPropertyBag() { }
	void readAttributes(std::string name);

public:
	HealthProperty pHealth;

	// Movement
	ForceProperty pForce;
	MaxVelocityProperty pMaxVelocity;
	DragFactorProperty pDragFactor;

	// Attributes
	LevelProperty pLevel;
	DefenceProperty pDefence;
	AttackDamageProperty pAttackDmg;
	AttackSpeedProperty pAttackSpd;
	MovementSpeedProperty pMovementSpd;

	WeaponProperty pWeapon;
};




