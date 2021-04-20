#pragma once

#include "Weapons/Weapon.h"
#include "Weapons/WeaponData.h"
#include "Collisions/Colliders/QuadCollider.h"

class MeleeWeaponData;
class Collider;


class MeleeWeapon : public Weapon
{
public:
	MeleeWeapon();
	~MeleeWeapon();

	void attack() override;

	void fastUpdate(float dt) override;
	void render() override;

	void equipt(const WeaponData* data) override;

	const BasicString& hitSoundLabel() override;
	const BasicString& missSoundLabel() override;

	void updateAimDirection(VectorF cursorPosition) override;

	VectorF& offset();
	Collider* getCollider() override { return &mCollider; }

	void flipSide() { mSwingDirection *= -1; }


private:
	void continueAttack(float dt);
	void endAttack();

	void updateQuadCollider();


private:
	MeleeWeaponData mMeleeData;

	Quad2D<float> mQuad;
	QuadCollider mCollider;

	int mSwingDirection;
	float mRotationSum;
	VectorF mAboutPoint;
};