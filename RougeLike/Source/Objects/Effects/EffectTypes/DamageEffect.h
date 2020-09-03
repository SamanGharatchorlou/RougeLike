#pragma once

#include "Effect.h"
#include "Objects/Properties/Attributes/Damage.h"

class DamageEffect : public Effect
{
public:
	DamageEffect() { }

	void fill(const PropertyMap& valueBag) override;

	void init() override;
	void fastUpdate(float dt) override { };
	void slowUpdate(float dt) override { };
	void render() override { }
	void exit() override;

	EffectType type() const override { return EffectType::Damage; }


private:
	Damage mDamage;
};