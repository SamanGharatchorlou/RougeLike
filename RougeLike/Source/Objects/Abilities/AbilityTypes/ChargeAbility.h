#pragma once

#include "Objects/Abilities/AbilityClasses/MeleeAbility.h"
#include "Collisions/WallCollisionTracker.h"


class ChargeAbility : public MeleeAbility
{
public:
	ChargeAbility() : mDistanceTravelled(0.0f) { };

	void activate(VectorF position) override;

	void fastUpdate(float dt) override;
	void slowUpdate(float dt) override;
	void render() override;
	void exit() override;

	AbilityType type() const override { return AbilityType::Charge; }


private:
	void applyEffects(Actor* actor, EffectPool* effectPool) override;

	void setCharging(bool isCharging);

	void setScaledQuad(float scale);
	void setQuadCollider();

	VectorF direction() const;
	double rotation() const;
	RectF renderRectFrontToColliderFront(const RectF& renderRect);


private:
	TimerF mTimer;
	VectorF mChargeTarget;
	float mDistanceTravelled;

	WallCollisionTracker mWallCollisions;
};