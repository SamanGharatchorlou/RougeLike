#pragma once

#include "EnemyState.h"

class EnemyDead : public EnemyState
{
public:
	EnemyDead() : mAlpha(alphaMax) { }

	void init() override;
	void handleInput() override { }
	void slowUpdate(float dt) override;
	void fastUpdate(float dt) override { };
	void render() override;
	void exit() override { };

	const Type type() const { return Type::Dead; }

private:
	double mAlpha;
};
