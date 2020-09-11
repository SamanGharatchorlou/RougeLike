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
		Stun,
		PreAttack,
		Attack,
		Charge,
		Dead,
		Exit,
		Count
	};

	EnemyState() : mEnemy(nullptr) { }

	void set(Enemy* enemy) { mEnemy = enemy; }
	virtual void render();

	virtual const Type type() const = 0;

protected:
	Enemy* mEnemy;
};


// The null state does nothing
struct EnemyNullState : public EnemyState
{
	void init() override { }
	void handleInput() override { }
	void slowUpdate(float /*dt*/) override { }
	void fastUpdate(float /*dt*/) override { }
	void render() override { }
	void exit() override { }

	const Type type() const { return Type::None; }
};

// EnemyManager will remove any enemy in this state
struct EnemyExit : public EnemyState
{
	void init() override { }
	void handleInput() override { }
	void slowUpdate(float /*dt*/) override { }
	void fastUpdate(float /*dt*/) override { }
	void render() override { }
	void exit() override { }

	const Type type() const { return Type::Exit; }
};


template<class T>
inline EnemyState::Type operator +(EnemyState::Type a, T b)
{
	int sum = static_cast<int>(a) + static_cast<int>(b);
	return static_cast<EnemyState::Type>(sum);
}