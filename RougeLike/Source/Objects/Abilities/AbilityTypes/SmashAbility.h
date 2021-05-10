#pragma once


#include "Objects/Abilities/AbilityClasses/RangedAbility.h"

#include "Collisions/WallCollisionTracker.h"
#include "Collisions/Colliders/QuadCollider.h"

class Texture;


class SmashAbility : public DirectionalAreaRangedAbility
{
public:
	SmashAbility(Texture* hammerTexture, RectF hammerRect);

	void init() override;
	void handleInput(const InputManager* input) override;

	void activate() override;

	void fastUpdate(float dt) override;
	void slowUpdate(float dt) override;
	void render() override;

	void exit() override;	
	
	const Collider* selectionCollider() const override { return &mSelectionCollider; }

	AbilityType type() const override { return AbilityType::Smash; }


private:
	void explode();

	void applyEffects(Actor* actor, EffectPool* effectPool) override;
	void applyHammerEffects(Actor* actor, EffectPool* effectPool);
	void applyExplosionEffects(Actor* actor, EffectPool* effectPool);

	void updateSelectionQuad();

private:
	struct Hammer
	{
		// NOTE: I replaced all the rect instances with a Quad, technically we dont need a rect
		// a quad should be able to handle all these things correectly... but I broke it...
		Texture* texture;
		Quad2D<float> quad;
		//RectF rect;
		VectorF direction;
		QuadCollider collider;
	} mHammer;

	//Texture* mHammerTexture;
	//RectF mHammerRect;

	bool mReachedTarget;
	VectorF mTargetPosition;
	//VectorF mHammerDirection;

	WallCollisionTracker mWallCollisions;

	

	Quad2D<float> mSelectionQuad;
	QuadCollider mSelectionCollider;
};