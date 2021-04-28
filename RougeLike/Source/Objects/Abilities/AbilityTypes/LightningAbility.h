#pragma once


#include "Objects/Abilities/AbilityClasses/RangedAbility.h"



class LightningAbility : public TargetAreaRangedAbility
{
public:
	LightningAbility() : mBounces(0), mBounceTime(0.0f) { };

	void init() override;

	void handleInput(const InputManager* input) override;
	void fastUpdate(float dt) override;
	void slowUpdate(float dt) override;

	bool activateOn(Actor* target, EffectPool* effectPool) override;
	void activate() override;

	void render() override;

	void exit() override;

	AbilityType type() const override { return AbilityType::Lightning; }

private:
	void applyEffects(Actor* actor, EffectPool* effectPool) override;

	void renderBoltBetweenTargets(const Actor* actorA, const Actor* actorB) const;

	Actor* getNextRandomTarget();

private:

	TimerF mTimer;
	VectorF mBoltCenter;

	Actor* mNextTarget;
	std::vector<Actor*> mTargetList;

	float mBounceTime;
	int mBounces;

	bool mFirstPass;
	bool mCountingLoop;
};