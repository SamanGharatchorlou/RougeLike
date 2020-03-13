#pragma once

#include "Enemy.h"

struct GameData;

class Imp : public Enemy
{
public:
	Imp(GameData* gameData);

	const EnemyType type() const override { return EnemyType::Imp; }
};