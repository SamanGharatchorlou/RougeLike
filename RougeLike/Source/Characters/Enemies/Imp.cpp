#include "pch.h"

#include "Imp.h"
#include "Game/GameData.h"


Imp::Imp(GameData* gameData) : Enemy(gameData)
{
	Enemy::init("Imp.xml");
}


void Imp::resolvePlayerWeaponCollisions()
{
	if (mCollider.hasCollided())
	{
		replaceState(EnemyState::Hit);
		mCollider.didCollide(false);
	}
}
