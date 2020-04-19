#pragma once

#include "EnemyState.h"

class EnemyWait : public EnemyState
{
public:
	EnemyWait(Enemy* enemy) : EnemyState(enemy) { }

	void init() override;
	void handleInput() override { }
	void fastUpdate(float dt) override { }
	void slowUpdate(float) override;
	void render() override;
	void exit() override { }

	const Type type() const { return Type::Wait; }

private:
	Timer<float> timer;
};