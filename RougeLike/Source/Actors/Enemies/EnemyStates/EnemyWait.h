#pragma once

#include "EnemyState.h"

class EnemyWait : public EnemyState
{
public:
	EnemyWait() : mWaitTime(2.0f) { }

	void setTime(float time) { mWaitTime = time; }

	void init() override;
	void handleInput() override { }
	void fastUpdate(float dt) override { }
	void slowUpdate(float) override;
	void render() override;

	void pause() override;
	void resume() override;
	void exit() override { };

	const Type type() const { return Type::Wait; }


private:
	float mWaitTime;
	TimerF timer;
};