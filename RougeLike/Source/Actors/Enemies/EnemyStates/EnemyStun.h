#pragma once

#include "EnemyState.h"
#include "Animations/Animator.h"

class EnemyStun: public EnemyState
{
public:
	EnemyStun() : mStunTime(-1.0f) { }

	void setTime(float time) { mStunTime = time; }

	void init() override;
	void handleInput() override { }
	void fastUpdate(float dt) override { }
	void slowUpdate(float) override;
	void render() override;

	void pause() override;
	void resume() override;
	void exit() override { };

	const Type type() const { return Type::Stun; }


private:
	float mStunTime;
	RectF mStunRect;
	Animator mStunAnimation;

	TimerF timer;
};