#include "pch.h"

#include "Imp.h"
#include "Game/GameData.h"


Imp::Imp(GameData* gameData) : Enemy(gameData)
{
	Enemy::init("Imp.xml");
}
