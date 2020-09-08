#pragma once

#include "Effect.h"

class InvunerabilityEffect : public Effect
{
public:
	InvunerabilityEffect() { }

	void fill(const PropertyMap& properties) override;

	void init() override;
	void fastUpdate(float dt) override { };
	void slowUpdate(float dt) override;
	void render() override { }
	void exit() override;

	EffectType type() const override { return EffectType::Invunerability; }


private:
	float mTime;
	TimerF mTimer;
};