#pragma once

#include "Damage.h"

class Health
{
public:
	Health() : hp(0.0f), maxHp(0.0f) { }
	Health(float maxHealth) : maxHp(maxHealth), hp(maxHealth) { }

	void set(float health) { hp = health; }
	float get() const { return hp; }
	float getPercentage() const { return hp / maxHp; }

	bool isDead() const { return hp <= 0; }
	void takeDamage(float damage) { hp -= damage; }
	void takeDamage(Damage damage) { hp -= damage.get(); }

	inline Health operator - (Damage damage) const
	{
		return Health(hp - damage.get());
	}

private:
	float hp;
	float maxHp;
};

