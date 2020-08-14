#pragma once

#include "../Enemy.h"

struct GameData;

class Devil : public Enemy
{
public:
	Devil();

	void init() override;

	const EnemyType type() const override { return EnemyType::Devil; }
};