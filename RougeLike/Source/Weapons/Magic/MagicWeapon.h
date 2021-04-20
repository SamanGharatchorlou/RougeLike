#pragma once

#include "Weapons/Weapon.h"
#include "Weapons/WeaponData.h"

#include "MagicProjectile.h"

class MagicWeapon : public Weapon
{
public:
	void attack() override;

	void fastUpdate(float dt) override;
	void render() override;

	void equipt(const WeaponData* data) override;


	void updateAimDirection(VectorF cursorPosition) override;

	const BasicString& hitSoundLabel() override;
	const BasicString& missSoundLabel() override;

	Collider* getCollider() override { return nullptr; }
	VectorF& offset() { return mOffset; }


private:
	MagicWeaponData mData;

	Queue<MagicProjectile> mProjectilePool;
	Queue<MagicProjectile> mActiveProjectiles;

	// doesnt use the mAttacking weapon member?

	// temp I have to override this as its pure virtual
	VectorF mOffset;
};