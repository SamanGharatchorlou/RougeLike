#pragma once

#include "Ability.h"

class Map;

class BlinkAbility: public AreaAbility
{
public:
	BlinkAbility(Map* map, float range) : mRange(range) { mMap = map; }

	void init(Animator animator) override;
	void activate(VectorF position) override;
	void activate(Actor* actor) override;
	void slowUpdate(float dt) override;
	void render() override;

	const TargetType targetType() const override { return TargetType::Area_Point; }


private:
	float mRange;
	VectorF mTarget;
};