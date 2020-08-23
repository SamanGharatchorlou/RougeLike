#pragma once

#include "Utilities/ObjectPool.h"
#include "Actors/Enemies/EnemyStates/EnemyState.h"


class EnemyStatePool : public ObjectPool<EnemyState, EnemyState::Type>
{
private:
	EnemyState* createNewObject(EnemyState::Type type) const override;
};