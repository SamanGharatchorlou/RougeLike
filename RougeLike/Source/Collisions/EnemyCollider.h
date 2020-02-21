#pragma once

#include "Collider.h"

class EnemyCollider : public Collider
{
public:
	EnemyCollder() : didAttack(false) { }

private:
	bool didAttack;
};