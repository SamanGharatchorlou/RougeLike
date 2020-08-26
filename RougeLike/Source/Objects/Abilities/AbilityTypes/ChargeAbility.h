#pragma once

#include "Objects/Abilities/AbilityClasses/TargetAbilities.h"
#include <unordered_set>

#include "Utilities/Quad2D.h"
#include "Collisions/WallCollisionTracker.h"



class ChargeAbility : public TargePositionAttackAbility
{
public:
	ChargeAbility() : mDistanceTravelled(0.0f) { setIsOnlyDirectional(true); };

	void activateAt(VectorF position, EffectPool* effectPool) override;
	void activateOn(Actor* actor, EffectPool* effectPool) override;

	void fastUpdate(float dt) override;
	void slowUpdate(float dt) override;
	void render() override;
	void exit() override;

	AbilityType type() const override { return AbilityType::Charge; }


private:
	void applyEffects(Actor* actor, EffectPool* effectPool);

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

	Quad2D<float> mQuad;
	std::unordered_set<Actor*> mHitList;
	WallCollisionTracker mWallCollisions;
};