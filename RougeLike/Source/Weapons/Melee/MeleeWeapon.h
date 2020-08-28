#pragma once

#include "Weapons/Weapon.h"
#include "Weapons/WeaponData.h"

// temp
#include "Utilities/Quad2D.h"
#include "Collisions/Colliders/QuadCollider.h"


class MeleeWeaponData;
class EffectCollider;
class Collider;


class MeleeWeapon : public Weapon
{
public:
	MeleeWeapon();
	~MeleeWeapon();

	void attack() override;

	void fastUpdate(float dt) override;
	void slowUpdate(float dt) override { };
	void render() override;

	void equipt(const WeaponData* data) override;

	const BasicString& hitSoundLabel() override;
	const BasicString& missSoundLabel() override;

	bool didHit() const override;
	bool containsCollider(Collider* collider) const;

	void updateAimDirection(VectorF cursorPosition) override;

	VectorF& offset() override;
	const float getAngle() const { return getRotation(mDirection); }
	Collider* getCollider() { return &mCollider; }
	const std::vector<Collider*> getColliders() const override;
	const std::vector<RectF> getRects() const { return mBlockRects; }

	void flipSide() { mSwingDirection *= -1; }

	std::vector<EffectCollider*> getEffectColliders() const { return mBlockColliders; }
	bool hasEffects() const;
	void addEffect(Effect* effect);

private:
	void updateWeaponBlocks();

	void continueAttack(float dt);
	void endAttack();


private:
	MeleeWeaponData mData;

	Quad2D<float> mQuad;
	QuadCollider mCollider;

	std::vector<RectF> mBlockRects;
	std::vector<EffectCollider*> mBlockColliders;

	int mSwingDirection;
	float mRotationSum;

	VectorF mAboutPoint;
};