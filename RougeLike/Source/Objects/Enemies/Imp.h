#pragma once

#include "Enemy.h"

struct GameData;
class AIPathMap;

class Imp : public Enemy
{
public:
	Imp(GameData* gameData);

	//void init() override;

	const EnemyType type() const override { return EnemyType::Imp; }

	RectF renderRect() const override;
};