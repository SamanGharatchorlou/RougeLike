#pragma once

#include "Ability.h"

class BlinkAbility: public AreaAbility
{
public:
	BlinkAbility(float range) : mRange(range) { }

	void init(Animator animator) override;
	void activate(VectorF position) override;
	void activate(Actor* actor) override;
	void slowUpdate(float dt) override;
	void render() override;

	const TargetType targetType() const override { return TargetType::Area_Point; }


private:
	void resetSize();


private:
	float mRange;
	VectorF mTarget;
	Timer<float> mTimer;
};