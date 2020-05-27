#pragma once

#include "Damage.h"
#include "Objects/Properties/Property.h"
#include "Utilities/Helpers.h"


class Health : public Property
{
public:
	Health() : hp(0), maxHp(0) { }
	Health(float maxHealth) : maxHp(maxHealth), hp(maxHealth) { }
	Health(float health, float maxHealth) : maxHp(maxHealth), hp(health) { }

	void increase(float health) { hp = clamp(hp + health, 0.0f, maxHp); }
	void reduce(Damage damage) { hp = clamp(hp - damage.value(), 0.0f, maxHp); }

	void setValue(float health) { hp = health; }
	const float value() const override { return hp; }

	float getPercentage() const { return hp / maxHp; }

	int getMax() const { return maxHp; }
	void increaseMax(float health) { hp += health; maxHp += health; }

	bool isDead() const { return hp <= 0; }

	void setFullHp() { hp = maxHp; }

private:
	float hp;
	float maxHp;
};
