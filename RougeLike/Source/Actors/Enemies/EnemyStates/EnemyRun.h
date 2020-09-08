#pragma once

#include "EnemyState.h"
#include "AI/Pathing/AIPathing.h"


class EnemyRun : public EnemyState
{
public:
	void init() override;
	void handleInput() override { }
	void slowUpdate(float) override;
	void fastUpdate(float dt) override;
	void render() override;
	void exit() override;

	void pause() override { };
	void resume() override;

	const Type type() const { return Type::Run; }

	bool canAttack() const;

	void updatePath(int pathLimit);
	Index nextTileIndex();

	const Path& path() const { return mPath; }


private:
	bool inAttackRange() const;
	bool inChaseRange() const;

	VectorF targetPosition() const;


private:
	Path mPath;
};
