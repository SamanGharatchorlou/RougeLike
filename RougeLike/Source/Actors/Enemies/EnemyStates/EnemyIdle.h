#pragma once

#include "EnemyState.h"


class EnemyIdle : public EnemyState
{
public:
	EnemyIdle(Enemy* enemy);
	EnemyIdle(Enemy* enemy, float time);

	void init() override;
	void handleInput() override { }
	void fastUpdate(float dt) override { };
	void slowUpdate(float) override;
	void render() override;
	void exit() override { }

	const Type type() const { return Type::Idle; }


private:
	bool canSeeAttackTarget() const;


private:
	Timer<float> mTimer;
	float mTime;
};