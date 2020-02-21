#pragma once

#include "Damage.h"

class Health
{
public:
	Health() : hp(0) { }
	Health(float health) : hp(health) { }

	void set(float health) { hp = health; }
	float get() const { return hp; }

	bool isDead() const { return hp <= 0; }
	void takeDamage(float damage) { hp -= damage; }

	inline Health operator - (Damage damage) const
	{
		return Health(hp - damage.get());
	}

private:
	float hp;
};

