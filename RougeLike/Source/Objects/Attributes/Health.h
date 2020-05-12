#pragma once

#include "Damage.h"
#include "Objects/Properties/Property.h"
#include "Utilities/Helpers.h"


class Health : public Property
{
public:
	Health() : hp(0), maxHp(0) { }
	Health(int maxHealth) : maxHp(maxHealth), hp(maxHealth) { }

	void setValue(float health) { hp = health; }
	const float value() const override { return (float)hp; }
	float getPercentage() const { return (float)hp / (float)maxHp; }

	void increase(int health) { hp = clamp(hp + health, 0, maxHp); }

	int getMax() const { return maxHp; }
	void increaseMax(Health health) { hp += health.value(); maxHp += health.value(); }

	bool isDead() const { return hp <= 0; }
	void takeDamage(int damage) { hp -= damage; }
	void takeDamage(Damage damage) { hp -= damage.value(); }

	inline Health operator - (Damage damage) const
	{
		return Health(hp - damage.value());
	}

	void setFullHp() { hp = maxHp; }

private:
	int hp;
	int maxHp;
};

