#pragma once

#include "AbilityStates.h"
#include "Animations/Animator.h"
#include "Objects/Abilities/Cooldown.h"

class Actor;
class EffectPool;
class Collider;
class InputManager;

enum class EffectType;


class Ability
{
public:
	Ability();
	virtual ~Ability();

	virtual void baseInit(const PropertyMap& properties, Animator animator);
	virtual void init() { }

	virtual void handleInput(const InputManager* input) { };
	virtual bool initiate(const InputManager* input) const = 0;

	virtual void fastUpdate(float dt) = 0;
	virtual void slowUpdate(float dt) = 0;
	virtual void render();
	virtual void exit() { };
	virtual void baseExit();

	virtual void activate() = 0;
	virtual bool activateOn(Actor* actor, EffectPool* effectPool) = 0;

	Actor* caster() const { return mCaster; }
	void setCaster(Actor* caster) { mCaster = caster; }

	Collider* collider() { return mCollider; }
	virtual const Collider* selectionCollider() const { return mCollider; }

	Cooldown& cooldown() { return mCooldown; }
	PropertyMap& properties() { return mProperties; }

	virtual AbilityTarget targetType() const = 0;
	virtual AbilityType type() const = 0;

	void setState(AbilityState state) { mState = state; }
	AbilityState state() const { return mState; }

	bool hasCompleted() const { return mCompleted; }
	bool shouldActivateCollisions() const { return mActivateCollisions; }

	BasicString name() const;

	void setSelectHighligh(RenderColour colour) { mSelectHighlight = colour; }


protected:
	void applyEffect(EffectType effectType, Actor* target, EffectPool* effectPool) const;



protected:
	Animator mAnimator;
	Cooldown mCooldown;
	PropertyMap mProperties;

	Actor* mCaster;

	RectF mRect; // remove this? melle has quad, ranged and self have rect?
	Collider* mCollider;

	RenderColour mSelectHighlight;

	AbilityState mState;

	bool mCompleted;
	bool mActivateCollisions;
};


void renderQuadOutline(const Quad2D<float>& quad, RenderColour colour);