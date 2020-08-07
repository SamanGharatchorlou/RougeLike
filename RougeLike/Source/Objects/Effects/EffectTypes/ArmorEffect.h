#pragma once

#include "Effect.h"
#include "Objects/Properties/Attributes/Armor.h"


class ArmorEffect : public Effect
{
public:
	ArmorEffect() { }

	void set(Armor armor) { mArmor = armor; }
	
	void clearData() override;

	void init() override;
	void fastUpdate(float dt) override { };
	void slowUpdate(float dt) override { };
	void render() override { }
	void exit() override { };

	EffectType type() const override { return EffectType::Armor; }


private:
	Armor mArmor;
};