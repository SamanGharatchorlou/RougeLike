#include "pch.h"
#include "PlayerAttackState.h"

#include "Actors/Player/Player.h"
#include "Weapons/Melee/MeleeWeapon.h"

#include "Input/InputManager.h"
#include "Input/Cursor.h"



PlayerAttackState::PlayerAttackState(Player* player)
{
	mWeapon = player->weapon();
}


void PlayerAttackState::init()
{
	mWeapon->attack();

}


void PlayerAttackState::fastUpdate(float dt)
{
	mWeapon->fastUpdate(dt);
}


void PlayerAttackState::slowUpdate(float dt)
{
	mWeapon->slowUpdate(dt);
}


void PlayerAttackState::render()
{
	mWeapon->render();
}


void PlayerAttackState::exit()
{

}


bool PlayerAttackState::finished()
{
	return !mWeapon->isAttacking();
}