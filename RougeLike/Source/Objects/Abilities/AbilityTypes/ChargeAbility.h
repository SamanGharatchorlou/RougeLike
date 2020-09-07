#pragma once

#include "Objects/Abilities/AbilityClasses/MeleeAbility.h"
#include "Collisions/WallCollisionTracker.h"
#include "Collisions/Colliders/QuadCollider.h"


class ChargeAbility : public MeleeAbility
{
public:
	ChargeAbility();

	void handleInput(const InputManager* input) override;

	void activate() override;

	void fastUpdate(float dt) override;
	void slowUpdate(float dt) override;
	void render() override;
	void exit() override;

	const Collider* selectionCollider() const override { return &mSelectionCollider; }

	AbilityType type() const override { return AbilityType::Charge; }



private:
	void applyEffects(Actor* actor, EffectPool* effectPool) override;

	void setCharging(bool isCharging);

	void updateQuad();
	void updateSelectionQuad();

	VectorF direction() const;
	double rotation() const;
	RectF renderRectFrontToColliderFront(const RectF& renderRect);



private:
	TimerF mTimer;
	VectorF mChargeTarget;
	float mDistanceTravelled;

	WallCollisionTracker mWallCollisions;

	Quad2D<float> mSelectionQuad;
	QuadCollider mSelectionCollider;
};