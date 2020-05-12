#pragma once

#include "Effect.h"

class HealEffect : public Effect
{
public:
	HealEffect(float heal) : mHeal(heal) { };

	void init() override;
	void fastUpdate(float dt) override { };
	void slowUpdate(float dt) override { };
	void render() override { }
	void exit() override { };

private:
	float mHeal;
};