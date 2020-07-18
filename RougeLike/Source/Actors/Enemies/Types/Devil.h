#pragma once

#include "../Enemy.h"

struct GameData;

class Devil : public Enemy
{
public:
	Devil();

	const EnemyType type() const override { return EnemyType::Devil; }
};