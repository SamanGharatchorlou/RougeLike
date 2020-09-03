#pragma once

#include "Objects/Properties/Attributes/Attribute.h"
#include "Damage.h"


class Health : public Attribute
{
public:
	Health() : hp(0), maxHp(0), mInvulnerable(false) { }
	Health(float maxHealth) : maxHp(maxHealth), hp(maxHealth) { }
	Health(float health, float maxHealth) : maxHp(maxHealth), hp(health) { }

	void init(float health) override { maxHp = health; setValue(health); }

	void increase(Health health) { setValue(clamp(hp + health.value(), 0.0f, maxHp)); }
	void reduce(Damage damage) 
	{ 
		if(!mInvulnerable)
			setValue(clamp(hp - damage.value(), 0.0f, maxHp)); 
	}

	void setValue(float health) { hp = health; mChanged = true; }
	const float value() const override { return hp; }

	float getPercentage() const { return hp / maxHp; }

	float getMax() const { return maxHp; }
	void increaseMax(float health) 
	{ 
		setValue(hp + health); 
		maxHp += health; 
	}

	bool isDead() const { return hp <= 0; }
	void setFullHp() { setValue(maxHp); }

	void setInvulnerablity(bool invulnerable) { mInvulnerable = invulnerable; }

	bool hasChanged() const { return mChanged; }
	void changedHandled() { mChanged = false; }


private:
	float hp;
	float maxHp;

	bool mInvulnerable;

	bool mChanged;
};
