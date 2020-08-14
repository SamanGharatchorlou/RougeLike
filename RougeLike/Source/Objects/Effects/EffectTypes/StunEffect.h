#pragma once

#include "Effect.h"


class StunEffect : public Effect
{
public:
	StunEffect() : mTime(0.0f) { };

	void fill(const PropertyMap& valueBag) override;
	void set(float time) { mTime = time; }

	void init() override;
	void fastUpdate(float dt) override { };
	void slowUpdate(float dt) override { }
	void render() override { }
	void exit() override;

	EffectType type() const override { return EffectType::Stun; }


private:
	float mTime;
};

