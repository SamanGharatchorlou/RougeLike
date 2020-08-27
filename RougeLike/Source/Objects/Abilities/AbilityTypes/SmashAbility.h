#pragma once


#include "Objects/Abilities/AbilityClasses/TargetAbilities.h"
#include "Utilities/Quad2D.h"

class Texture;


class SmashAbility : public TargePositionAttackAbility
{
public:
	SmashAbility(Texture* hammerTexture, RectF hammerRect);

	void activateAt(VectorF position, EffectPool* pool) override;
	void activateOn(Actor* target, EffectPool* pool) override;

	void fastUpdate(float dt) override;
	void slowUpdate(float dt) override;
	void render() override;

	void exit() override;

	AbilityType type() const override { return AbilityType::Smash; }


private:
	void applyHammerEffects(Actor* actor, EffectPool* effectPool);
	void applyExplosionEffects(Actor* actor, EffectPool* effectPool);


private:
	Texture* mHammerTexture;
	RectF mHammerRect;

	bool mReachedTarget;
	VectorF mTargetPosition;
	VectorF mHammerDirection;

	Quad2D<float> mQuad;
	std::unordered_set<Actor*> mHitList;
};