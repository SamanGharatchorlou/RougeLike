#pragma once

#include "States/State.h"

#include "EnemyState.h"

class Enemy;

class EnemyAlert : public EnemyState
{
public:
	EnemyAlert(Enemy* enemy) : EnemyState(enemy) { }

	void init() override;
	void handleInput() override { }
	void slowUpdate(float) override;
	void fastUpdate(float dt) override { };
	void render() override;
	void exit() override { }

	const Type type() const { return Type::Alert; }

private:
	Timer<float> timer;
};