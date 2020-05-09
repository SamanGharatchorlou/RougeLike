#pragma once

#include "Damage.h"
#include "Objects/Properties/PropertyBase.h"
#include "Utilities/Helpers.h"

class Health : public PropertyBase
{
public:
	Health() : PropertyBase(PropertyType::Health), hp(0), maxHp(0) { }
	Health(int maxHealth) : PropertyBase(PropertyType::Health), maxHp(maxHealth), hp(maxHealth) { }

	void set(int health) { hp = health; }
	float value() const override { return (float)hp; }
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

