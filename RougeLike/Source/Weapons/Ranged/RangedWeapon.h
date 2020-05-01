#pragma once

#include "Weapons/Weapon.h"

#include "Quiver.h"
#include "Utilities/Queue.h"

struct RangedWeaponData;
class Projectile;


class RangedWeapon : public Weapon
{
public:
	RangedWeapon();
	~RangedWeapon();

	void attack() override;

	void fastUpdate(float dt) override;
	void slowUpdate(float dt) override;

	// TODO: Unimplemented functions
	const std::string& hitSoundLabel() const override { return ""; };
	const std::string& missSoundLabel() const override { return ""; };
	bool didHit() const override { return false; }
	// Unimplemented functions

	void updateAnchor(VectorF anchor) override;
	void updateAimDirection(VectorF cursorPosition) override;

	void equipt(const WeaponData* data) override;
	void updateStats(const PlayerPropertyBag* bag) override;

	const std::vector<Collider*> getColliders() override;

	void render() override;

	//TEMP
	const std::vector<RectF> getRects() const { return std::vector<RectF>(); }

private:
	const RangedWeaponData* mData;

	Quiver quiver;

	//Queue<Projectile*> travelingProjectiles;

	std::list<Projectile*> travelingProjectiles;
};