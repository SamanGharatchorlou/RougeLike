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

	const BasicString& hitSoundLabel() override;
	const BasicString& missSoundLabel() override;

	bool didHit() const override;
	bool containsCollider(Collider* collider) const;

	void updateAimDirection(VectorF cursorPosition) override;

	// Stats
	// TODO: can remove some of these? just pull from the data?
	const MeleeWeaponData* getData() const { return mMeleeData; }
	const float getAngle() const { return getRotation(mDirection); }
	const float maxSwingAngle() const;
	const float swingSpeed() const { return mSwingSpeed; }
	const float knockbackDistance() const;

	const std::vector<Collider*> getColliders() override;
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
	const MeleeWeaponData* mMeleeData;

	std::vector<RectF> mBlockRects;
	std::vector<EffectCollider*> mBlockColliders;

	int mSwingDirection;
	float mRotationSum;
	float mSwingSpeed;

	VectorF mAboutPoint;

#if DRAW_PLAYER_RECTS
	VectorF weaponVectorTest;
#endif
};