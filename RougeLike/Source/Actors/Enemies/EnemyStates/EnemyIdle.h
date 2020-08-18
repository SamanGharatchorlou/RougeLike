#pragma once

#include "EnemyState.h"


class EnemyIdle : public EnemyState
{
public:
	EnemyIdle();

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
	Timer<float> mTimer;
	float mTime;
};