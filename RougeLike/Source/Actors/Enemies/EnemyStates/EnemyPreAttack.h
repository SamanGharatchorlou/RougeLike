#pragma once

#include "EnemyState.h"


class EnemyPreAttack : public EnemyState
{
public:
	EnemyPreAttack() : mAttackWaitTime(-1.0f) { }
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
	TimerF timer;
	float mAttackWaitTime;
};


