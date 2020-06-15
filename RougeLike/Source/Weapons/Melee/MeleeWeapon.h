#pragma once

#include "Weapons/Weapon.h"

#if _DEBUG
#include "Debug/DebugDraw.h"
#endif

struct MeleeWeaponData;
class EffectCollider;


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

	const std::string& hitSoundLabel() override;
	const std::string& missSoundLabel() override;

	bool didHit() const override;

	void updateAimDirection(VectorF cursorPosition) override;


	//void addEffect(Effect* effect) override;
	//void clearEffect() override;


	// Stats
	const MeleeWeaponData* getData() const { return mMeleeData; }
	const float getAngle() const { return getRotation(mDirection); }
	const float maxSwingAngle() const;
	const float swingSpeed() const { return mSwingSpeed; }
	const float knockbackDistance() const;

	const std::vector<Collider*> getColliders() override;
	const std::vector<RectF> getRects() const { return mBlockRects; }

	void flipSide() { mSwingDirection *= -1; }

	std::vector<EffectCollider*> getEffectColliders() const { return mBlockColliders; }


private:
	void updateWeaponBlocks();

	void continueAttack(float dt);
	void endAttack();


private:
	const MeleeWeaponData* mMeleeData;

	std::vector<RectF> mBlockRects;
	std::vector<EffectCollider*> mBlockColliders;

	int mSwingDirection;
	float mRotationSum;
	float mSwingSpeed;

	VectorF aboutPoint;

	std::vector<Collider*> hitColliders;

#if DRAW_PLAYER_RECTS
	VectorF weaponVectorTest;
#endif
};