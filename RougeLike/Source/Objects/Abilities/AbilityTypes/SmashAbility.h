#pragma once


#include "Objects/Abilities/AbilityClasses/RangedAbility.h"
#include "Utilities/Quad2D.h"

class Texture;


class SmashAbility : public DirectionalAreaRangedAbility
{
public:
	SmashAbility(Texture* hammerTexture, RectF hammerRect);

	void activate(VectorF position) override;

	void fastUpdate(float dt) override { }
	void slowUpdate(float dt) override;
	void render() override;

	void exit() override;

	AbilityType type() const override { return AbilityType::Smash; }


private:
	void applyEffects(Actor* actor, EffectPool* effectPool);
	void applyHammerEffects(Actor* actor, EffectPool* effectPool);
	void applyExplosionEffects(Actor* actor, EffectPool* effectPool);


private:
	Texture* mHammerTexture;
	RectF mHammerRect;

	bool mReachedTarget;
	VectorF mTargetPosition;
	VectorF mHammerDirection;

	Quad2D<float> mQuad;
};