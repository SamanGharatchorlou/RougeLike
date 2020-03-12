#pragma once

#include "Damage.h"

class Health
{
public:
	Health() : hp(0), maxHp(0) { }
	Health(int maxHealth) : maxHp(maxHealth), hp(maxHealth) { }

	void set(int health) { hp = health; }
	int value() const { return hp; }
	float getPercentage() const { return (float)hp / (float)maxHp; }

	void increase(int health)
	{
		hp += health;

		// TODO: how can I use clamp here, there's no pch included...
		hp = clamp(hp, 0, maxHp);
	}

	int getMax() const { return maxHp; }
	void increaseMax(Health health) { hp += health.value(); maxHp += health.value(); }

	bool isDead() const { return hp <= 0; }
	void takeDamage(int damage) { hp -= damage; }
	void takeDamage(Damage damage) { hp -= damage.value(); }

	inline Health operator - (Damage damage) const
	{
		return Health(hp - damage.value());
	}

private:
	int hp;
	int maxHp;
};

