#pragma once

#include "Effect.h"
#include "Objects/Properties/Attributes/Damage.h"

class DamageEffect : public Effect
{
public:
	DamageEffect() : mReact(true) { }
	DamageEffect(Damage damage) : mDamage(damage), mReact(true) { };

	void fill(const PropertyMap& valueBag) override;
	void set(Damage damage, bool shouldReact = true) { mDamage = damage; mReact = shouldReact;}

	void init() override;
	void fastUpdate(float dt) override { };
	void slowUpdate(float dt) override { };
	void render() override { }
	void exit() override;

	EffectType type() const override { return EffectType::Damage; }

	bool shouldReact() const { return mReact; }


private:
	Damage mDamage;
	bool mReact;
};