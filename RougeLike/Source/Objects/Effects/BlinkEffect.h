#pragma once

#include "Effect.h"

class BlinkEffect : public Effect
{
public:
	BlinkEffect(VectorF target) : mTarget(target) { };

	void init() override;
	void fastUpdate(float dt) override { };
	void slowUpdate(float dt) override;
	void render() override { }
	void exit() override;


private:
	VectorF mTarget;
};