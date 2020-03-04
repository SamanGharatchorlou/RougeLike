#pragma once

#include "Weapons/Weapon.h"
#include "Projectile.h"

struct RangedWeaponData;


class RangedWeapon : public Weapon
{
public:
	RangedWeapon();
	~RangedWeapon();

	void attack() override { } //{ mAttacking = true; }

	void update(float dt) override { }

	void updateAnchor(VectorF anchor) override;
	void updateAimDirection(Camera* camera, VectorF cursorPosition) override;

	void equipt(const WeaponData* data) override;
	void updateStats(const PlayerPropertyBag* bag) override;

	const std::vector<Collider*> getColliders() const override;
	void setColliderActivite(bool isActive) override;

	void render(Camera* camera) override;

private:
	const RangedWeaponData* mData;

	std::vector<Projectile> projectiles;
};