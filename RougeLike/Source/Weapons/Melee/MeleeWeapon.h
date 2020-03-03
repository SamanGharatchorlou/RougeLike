#pragma once

#include "Weapons/Weapon.h"

class DamageCollider;
class Camera;

class MeleeWeapon : public Weapon
{
public:
	MeleeWeapon();
	~MeleeWeapon();

	void updateAnchor(VectorF anchor) override;
	void updatePommelToCursor(Camera* camera, VectorF cursorPosition) override;

	void equipt(const WeaponData* data) override;
	void updateStats(const PlayerPropertyBag* bag) override;

	void rotate(double theta);
	const double getAngle() const { return getRotation(mDirection); }
	const double maxSwingAngle() const;
	const double swingSpeed() const { return mPlayerSwingSpeed; }

	const std::vector<Collider*> getColliders() const override;
	void setColliderActivite(bool isActive) override;

	const std::vector<RectF> getRects() const { return mBlockRects; }

	void flipSide() { mSwingDirection *= -1; }

	void render(Camera* camera) override;


private:
	void updateWeaponBlocks();


private:
	std::vector<RectF> mBlockRects;
	std::vector<DamageCollider*> mBlockColliders;

	VectorF mDirection;
	int mSwingDirection;
	double mPlayerSwingSpeed;
};