#pragma once

#include "Weapons/Weapon.h"

struct MeleeWeaponData;
class DamageCollider;


class MeleeWeapon : public Weapon
{
public:
	MeleeWeapon();
	~MeleeWeapon();

	void attack() override;

	void fastUpdate(float dt) override;
	void slowUpdate(float dt) override { };

	void updateAnchor(VectorF anchor) override;
	void updateAimDirection(VectorF cursorPosition) override;

	void equipt(const WeaponData* data) override;
	void updateStats(const PlayerPropertyBag* bag) override;

	const float getAngle() const { return getRotation(mDirection); }
	const float maxSwingAngle() const;
	const float swingSpeed() const { return mPlayerSwingSpeed; }

	const std::vector<Collider*> getColliders() const override;
	void setColliderActivite(bool isActive) override;

	const std::vector<RectF> getRects() const { return mBlockRects; }

	void flipSide() { mSwingDirection *= -1; }

	void render() override;


private:
	void updateWeaponBlocks();


private:
	const MeleeWeaponData* mData;

	std::vector<RectF> mBlockRects;
	std::vector<DamageCollider*> mBlockColliders;
	
	bool mAttacking;

	int mSwingDirection;
	float mRotationSum;
	float mPlayerSwingSpeed;
};