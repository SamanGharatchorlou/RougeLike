#include "pch.h"
#include "Devil.h"


Devil::Devil(GameData* gameData) : Enemy(gameData) { }


void Devil::init()
{
	Enemy::init("Devil");
}
