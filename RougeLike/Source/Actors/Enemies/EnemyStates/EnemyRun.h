#pragma once

#include "EnemyState.h"
#include "AI/AIPathing.h"


class EnemyRun : public EnemyState
{
public:
	EnemyRun(Enemy* enemy);

	void init() override;
	void handleInput() override { }
	void slowUpdate(float) override;
	void fastUpdate(float dt) override;
	void render() override;
	void exit() override { }

	void pause() override { };
	void resume() override;

	const Type type() const { return Type::Run; }

	bool inAttackRange() const;

	void updatePath();
	Index nextTileIndex();

	const Path& path() const { return mPath; }


private:
	bool inChaseRange() const;


private:
	Path mPath;
};
