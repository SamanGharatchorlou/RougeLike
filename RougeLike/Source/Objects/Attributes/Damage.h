#pragma once

#include "Property.h"

class Damage : public Property
{
public:
	Damage() : attackDamage(0) { }
	Damage(float dmg) : attackDamage(dmg) { }

	void set(float damage) { attackDamage = damage; }
	const float value() const { return attackDamage; }

	inline Damage operator + (Damage damage) const
	{
		return Damage(attackDamage + damage.value());
	}

	inline Damage& operator += (Damage damage)
	{
		attackDamage += damage.value();
		return *this;
	}

	// TODO: add effect damage e.g. fire, ice etc
private:
	float attackDamage;
};