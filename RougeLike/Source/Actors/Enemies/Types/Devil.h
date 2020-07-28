#pragma once

#include "../Enemy.h"

struct GameData;

class Devil : public Enemy
{
public:
	Devil();

	void init(TextureManager* textureManager, EffectPool* effects) override;

	const EnemyType type() const override { return EnemyType::Devil; }
};