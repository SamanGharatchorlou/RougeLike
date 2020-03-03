#pragma once

#include "Weapon.h"

class DamageCollider;


class MeleeWeapon : public Weapon
{
public:
	MeleeWeapon(GameData* gameData);
	~MeleeWeapon();

	void fastUpdate(VectorF anchorPosition);

	void equipt(const WeaponData* data);
	void updateStats(const PlayerPropertyBag* bag);

	void rotate(double theta);
	const double getAngle() const { return getRotation(mDirection); }
	const double maxSwingAngle() const;
	const double swingSpeed() const { return mPlayerSwingSpeed; }

	const std::vector<Collider*> getColliders() const;

	const std::vector<RectF> getRects() const { return mBlockRects; }

	void flipSide() { mSwingDirection *= -1; }

	void render();


private:
	void updateWeaponBlocks();


private:
	std::vector<RectF> mBlockRects;
	std::vector<DamageCollider*> mBlockColliders;

	VectorF mDirection;
	int mSwingDirection;
	double mPlayerSwingSpeed;
};