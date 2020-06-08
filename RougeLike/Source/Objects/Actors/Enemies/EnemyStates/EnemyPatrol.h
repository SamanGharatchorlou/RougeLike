#pragma once

#include "EnemyState.h"

class EnemyPatrol : public EnemyState
{
public:
	EnemyPatrol(Enemy* enemy);

	void init() override;
	void handleInput() override { }
	void slowUpdate(float) override;
	void fastUpdate(float dt) override;
	void render() override;
	void exit() override;

	void resume() override;

	const Type type() const { return Type::Patrol; }


private:
	Vector2D<int> findYFloorTileRange(int xIndex) const;

	void setPatrolPoint();
	bool hasReachedPositionTarget() const;
	bool canSeeAttackTarget() const;


private:
	VectorF mPositionTarget;
};

