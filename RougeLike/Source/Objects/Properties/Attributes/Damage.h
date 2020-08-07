#pragma once

#include "Objects/Properties/Attributes/Attribute.h"

class Damage : public Attribute
{
public:
	Damage() : attackDamage(0) { }
	Damage(float dmg) : attackDamage(dmg) { }

	void setValue(float damage) { attackDamage = damage; }
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

private:
	float attackDamage;
};