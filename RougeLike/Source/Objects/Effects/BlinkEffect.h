#pragma once

#include "Effect.h"

class BlinkEffect : public Effect
{
public:
	BlinkEffect() { };

	void set(VectorF target) { mTarget = target; }

	void fillData(const EffectPropertyBag* properties) override;
	void clearData() override;

	void init() override;
	void fastUpdate(float dt) override { };
	void slowUpdate(float dt) override;
	void render() override { }
	void exit() override;

	EffectType type() const override { return EffectType::Blink; }

private:
	VectorF mTarget;
};