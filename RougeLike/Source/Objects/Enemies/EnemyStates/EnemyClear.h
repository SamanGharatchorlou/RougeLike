#pragma once

#include "EnemyState.h"

class EnemyClear : public EnemyState
{
public:
	EnemyClear(Enemy* enemy);

	void init() override;
	void handleInput() override { }
	void slowUpdate(float dt) override;
	void fastUpdate(float dt) override;
	void render() override;
	void exit() override;

	const Type type() const { return Type::Hit; }
};
