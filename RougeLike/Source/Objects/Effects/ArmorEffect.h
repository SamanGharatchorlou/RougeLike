#pragma once

#include "Effect.h"

class ArmorEffect : public Effect
{
public:
	ArmorEffect(float armor) : mArmor(armor) { };

	void init() override;
	void fastUpdate(float dt) override { };
	void slowUpdate(float dt) override { };
	void render() override { }
	void exit() override { };

private:
	float mArmor;
};