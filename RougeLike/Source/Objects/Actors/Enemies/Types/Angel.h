#pragma once

#include "../Enemy.h"

struct GameData;

class Angel : public Enemy
{
public:
	Angel(GameData* gameData);

	void init();

	const EnemyType type() const override { return EnemyType::Angel; }

	RectF renderRect() const override;
};