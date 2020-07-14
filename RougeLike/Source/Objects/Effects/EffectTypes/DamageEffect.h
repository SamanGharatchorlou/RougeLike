#pragma once

#include "Effect.h"
#include "Objects/Attributes/Damage.h"


class DamageEffect : public Effect
{
public:
	DamageEffect() { }
	DamageEffect(Damage damage) : mDamage(damage) { };

	void set(Damage damage) { mDamage = damage; }

	void clearData() override;

	void init() override;
	void fastUpdate(float dt) override { };
	void slowUpdate(float dt) override { };
	void render() override { }
	void exit() override { }

	EffectType type() const override { return EffectType::Damage; }


private:
	Damage mDamage;
};