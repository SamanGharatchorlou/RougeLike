#pragma once

#include "Effect.h"
#include "Objects/Properties/Attributes/Health.h"


class HealEffect : public Effect
{
public:
	HealEffect() { }

	void fill(const PropertyMap& properties) override;

	void init() override;
	void fastUpdate(float dt) override { };
	void slowUpdate(float dt) override { };
	void render() override { }
	void exit() override;

	EffectType type() const override { return EffectType::Heal; }


private:
	Health mHeal;
};