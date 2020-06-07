#include "pch.h"
#include "Angel.h"

Angel::Angel(GameData* gameData) : Enemy(gameData) { }

void Angel::init()
{
	Enemy::init("Angel");
}
