#include "pch.h"
#include "Imp.h"

Imp::Imp(GameData* gameData) : Enemy(gameData) { }

void Imp::init()
{
	Enemy::init("Imp");
}
