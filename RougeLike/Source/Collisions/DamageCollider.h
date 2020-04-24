#pragma once

#include "Collider.h"
#include "Characters/Attributes/Damage.h"


class DamageCollider : public Collider
{
public:
	void set(Damage damage, float knockback) { mDamage = damage; mKnockbackDistance = knockback; }

	void setDamage(Damage damage) { mDamage = damage; }
	const Damage damage() const { return mDamage; }

	const float knockbackforce() const { return mKnockbackDistance; }

protected:
	Damage mDamage;
	float mKnockbackDistance;

};