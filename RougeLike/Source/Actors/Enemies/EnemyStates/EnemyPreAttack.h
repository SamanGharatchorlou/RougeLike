#pragma once

#include "EnemyState.h"


class EnemyPreAttack : public EnemyState
{
public:
	void enter() override { }
	void init() override;
	void handleInput() override { }
	void fastUpdate(float dt) override { };
	void slowUpdate(float dt) override;
	void render() override;
	void exit() override { };
	void resume() override;

	const Type type() const { return Type::PreAttack; }

private:
	bool inAttackRange() const;


private:
	Timer<float> timer;
	float mAttackWaitTime;
};


