//#pragma once
//
//#include "Ability.h"
//
//class SlowAbility : public Ability
//{
//public:
//	SlowAbility() { }
//
//	void fillValues(ValueMap& values) override;
//
//	void activate(Actor* target, EffectPool* effectPool) override;
//	void slowUpdate(float dt) override { }
//
//	const TargetType targetType() const override { return TargetType::Single_Enemy; }
//
//private:
//	float mSlowFactor;
//};