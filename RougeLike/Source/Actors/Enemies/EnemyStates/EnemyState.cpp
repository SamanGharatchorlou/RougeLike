#include "pch.h"
#include "EnemyState.h"

#include "Actors/Enemies/Enemy.h"


void EnemyState::render()
{
	mEnemy->renderCharacter();
}