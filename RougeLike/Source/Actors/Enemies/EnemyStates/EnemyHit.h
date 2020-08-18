#pragma once

#include "EnemyState.h"

class EnemyHit : public EnemyState
{
public:
	void init() override;
	void handleInput() override { }
	void fastUpdate(float dt) override { };
	void slowUpdate(float dt) override;
	void render() override;
	void exit() override;
	void resume() override;

	const Type type() const { return Type::Hit; }
};
