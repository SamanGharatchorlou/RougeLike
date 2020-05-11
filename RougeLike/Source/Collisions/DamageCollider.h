#pragma once

#include "Collider.h"
#include "Objects/Attributes/Damage.h"


class DamageCollider : public Collider
{
public:
	void initDamage(Damage damage, float knockback) { mDamage = damage; mKnockbackDistance = knockback; }

	const Damage damage() const { return mDamage; }
	const float knockbackforce() const { return mKnockbackDistance; }

protected:
	Damage mDamage;
	float mKnockbackDistance;

};