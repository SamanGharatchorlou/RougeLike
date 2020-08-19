#pragma once

#include "EnemyState.h"

class Map;

class EnemyPatrol : public EnemyState
{
public:
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


	//bool canMove(VectorF velocity, float dt);


private:
	VectorF mPositionTarget;
	//Index mIndexTarget;
};

