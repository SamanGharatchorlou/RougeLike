#pragma once

#include "EnemyState.h"

class Enemy;

class EnemyDead : public EnemyState
{
public:
	EnemyDead(Enemy* enemy);

	void init() override;
	void handleInput() override { }
	void slowUpdate(float dt) override;
	void fastUpdate(float dt) override { };
	void render() override;
	void exit() override { };

	const Type type() const { return Type::Dead; }

private:
	Uint8 mAlpha;
};
