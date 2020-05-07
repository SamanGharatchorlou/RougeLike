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
	void render() override;

	bool canPlayHitSound() { return mCanPlayHitSound; }
	const std::string& hitSoundLabel() override;
	const std::string& missSoundLabel() override;

	bool didHit() const override;

	void updateAnchor(VectorF anchor) override;
	void updateAimDirection(VectorF cursorPosition) override;

	void equipt(const WeaponData* data) override;
	void updateStats(const PlayerPropertyBag* bag) override;

	// Stats
	const float getAngle() const { return getRotation(mDirection); }
	const float maxSwingAngle() const;
	const float swingSpeed() const { return mSwingSpeed; }
	const float knockbackDistance() const;

	const std::vector<Collider*> getColliders() override;
	const std::vector<RectF> getRects() const { return mBlockRects; }

	void flipSide() { mSwingDirection *= -1; }



private:
	void updateWeaponBlocks();

	void continueAttack(float dt);
	void endAttack();


private:
	const MeleeWeaponData* mData;

	std::vector<RectF> mBlockRects;
	std::vector<DamageCollider*> mBlockColliders;

	int mSwingDirection;
	float mRotationSum;
	float mSwingSpeed;
};