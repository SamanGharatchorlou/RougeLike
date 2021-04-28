#pragma once

#include "../Enemy.h"
#include "Collisions/WallCollisionTracker.h"


struct GameData;

class Orc : public Enemy
{
public:
	Orc();

	const EnemyType type() const override { return EnemyType::Orc; }

	void meleeAttack() override;

	bool canAttck() const override;
	bool isAttacking() const override;

};