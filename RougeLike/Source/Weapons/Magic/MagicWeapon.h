#pragma once

#include "Weapons/Weapon.h"
#include "Weapons/WeaponData.h"
#include "Utilities/LinkedList.h"

class MagicProjectile;

class MagicWeapon : public Weapon
{
public:
	bool canAttack() override { return true; }
	void attack() override;

	void slowUpdate(float dt) override;
	void fastUpdate(float dt) override;
	void render() override;

	void unequipt();
	void equipt(const WeaponData* data) override;

	void setPosition(VectorF position) override { mRect.SetCenter(position); }
	void updateAimDirection(VectorF cursorPosition) override;

	const BasicString& hitSoundLabel() override;
	const BasicString& missSoundLabel() override;

	std::vector<Collider*> getColliders() override;
	VectorF& offset() { return mOffset; }

	const WeaponData* getData() override { return &mData; }

	std::vector<Effect*> getEffects(EffectPool* effectPool);

private:
	MagicWeaponData mData;

	MagicProjectile* mPoolHead;
	Queue<MagicProjectile*> mProjectilePool;
	Queue<MagicProjectile*> mActiveProjectiles;

	// doesnt use the mAttacking weapon member?

	// temp I have to override this as its pure virtual
	VectorF mOffset;
};