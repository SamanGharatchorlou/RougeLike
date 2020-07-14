#pragma once

#include "EnemyState.h"

class Map;

class EnemyPatrol : public EnemyState
{
public:
	EnemyPatrol(Enemy* enemy);

	void init() override;
	void handleInput() override { }
	void slowUpdate(float) override;
	void fastUpdate(float dt) override;
	void render() override;
	void exit() override { };

	void resume() override;

	const Type type() const { return Type::Patrol; }


private:
	void setPatrolPoint();
	bool hasReachedPositionTarget() const;
	bool canSeeAttackTarget() const;


private:
	VectorF mPositionTarget;
};

