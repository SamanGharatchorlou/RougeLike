#pragma once

#include "States/State.h"

class Enemy;

struct EnemyState : public State
{
	enum Type
	{
		None,
		Wait,
		Idle,
		Run,
		Patrol,
		Alert,
		Hit,
		PreAttack,
		Attack,
		Dead
	};

	EnemyState(Enemy* enemy) : mEnemy(enemy) { }

	virtual const Type type() const = 0;

protected:
	Enemy* mEnemy;
};


// The null state does nothing
struct EnemyNullState : public EnemyState
{
	EnemyNullState() : EnemyState(nullptr) { }

	void init() override { }
	void handleInput() override { }
	void slowUpdate(float /*dt*/) override { }
	void fastUpdate(float /*dt*/) override { }
	void render() override { }
	void exit() override { }

	const Type type() const { return Type::None; }
};