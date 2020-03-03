#pragma once

#include "Weapons/Weapon.h"


class RangedWeapon : public Weapon
{
public:
	RangedWeapon();
	~RangedWeapon();

	void updateAnchor(VectorF anchor) override;
	void updatePommelToCursor(Camera* camera, VectorF cursorPosition) override;

	void equipt(const WeaponData* data) override;
	void updateStats(const PlayerPropertyBag* bag) override;

	const std::vector<Collider*> getColliders() const override;
	void setColliderActivite(bool isActive) override;

	void render(Camera* camera) override;

private:


};