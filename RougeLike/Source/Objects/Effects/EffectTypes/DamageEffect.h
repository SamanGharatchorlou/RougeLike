#pragma once

#include "Effect.h"
#include "Objects/Properties/PropertyTypes/Damage.h"


class DamageEffect : public Effect
{
public:
	DamageEffect() { }
	DamageEffect(Damage damage) : mDamage(damage) { };

	void fill(const ValueBag& valueBag) override;
	void set(Damage damage) { mDamage = damage; }

	void init() override;
	void fastUpdate(float dt) override { };
	void slowUpdate(float dt) override { };
	void render() override { }
	void exit() override;

	EffectType type() const override { return EffectType::Damage; }


private:
	Damage mDamage;
};