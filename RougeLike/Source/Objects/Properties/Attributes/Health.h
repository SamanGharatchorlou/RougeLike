#pragma once

#include "Objects/Properties/Attributes/Attribute.h"
#include "Damage.h"


class Health : public Attribute
{
public:
	Health() : hp(0), maxHp(0) { }
	Health(float maxHealth) : maxHp(maxHealth), hp(maxHealth) { }
	Health(float health, float maxHealth) : maxHp(maxHealth), hp(health) { }

	void init(float health) override { maxHp = health; hp = health; }

	void increase(Health health) { hp = clamp(hp + health.value(), 0.0f, maxHp); }
	void reduce(Damage damage) { hp = clamp(hp - damage.value(), 0.0f, maxHp); }

	void setValue(float health) { hp = health; }
	const float value() const override { return hp; }

	float getPercentage() const { return hp / maxHp; }

	float getMax() const { return maxHp; }
	void increaseMax(float health) { hp += health; maxHp += health; }

	bool isDead() const { return hp <= 0; }

	void setFullHp() { hp = maxHp; }

private:
	float hp;
	float maxHp;
};