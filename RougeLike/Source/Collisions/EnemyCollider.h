#pragma once

#include "DamageCollider.h"


class EnemyCollider : public DamageCollider
{
public:
	EnemyCollider() : mProcessedAttack(false) { }

	void hasProcessedAttack(bool wasProcessed) { mProcessedAttack = wasProcessed; }
	bool hasProcessedAttack() const { return mProcessedAttack; }

private:
	bool mProcessedAttack;
};