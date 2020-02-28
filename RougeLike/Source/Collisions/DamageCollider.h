#pragma once

#include "Collider.h"
#include "Characters/Attributes/Damage.h"


class DamageCollider : public Collider
{
public:
	void setDamage(Damage damage) { mDamage = damage; }
	const Damage getDamage() const { return mDamage; }


protected:
	Damage mDamage;
};