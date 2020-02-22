#pragma once

#include "Collider.h"

class EnemyCollider : public Collider
{
public:
	EnemyCollider() : didAttack(false) { }

private:
	bool didAttack;
};