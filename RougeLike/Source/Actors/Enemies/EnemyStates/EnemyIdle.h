#pragma once

#include "EnemyState.h"


class EnemyIdle : public EnemyState
{
public:
	EnemyIdle() : mTime(-1.0f) { }

	void setTime(float time);

	void init() override;
	void handleInput() override { }
	void fastUpdate(float dt) override { };
	void slowUpdate(float) override;
	void render() override;
	void exit() override { mTimer.stop(); }
	void resume() override;

	const Type type() const { return Type::Idle; }


private:
	bool canSeeAttackTarget() const;


private:
	TimerF mTimer;
	float mTime;
};