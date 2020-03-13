#pragma once

#include "EnemyState.h"


class EnemyPreAttack : public EnemyState
{
public:
	EnemyPreAttack(Enemy* enemy);

	void init() override;
	void handleInput() override { }
	void slowUpdate(float dt) override;
	void fastUpdate(float dt) override { };
	void render() override;
	void exit() override { };

	const Type type() const { return Type::PreAttack; }

private:
	bool inAttackRange() const;


private:
	Timer<float> timer;
};


