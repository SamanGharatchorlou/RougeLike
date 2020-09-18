#include "pch.h"
#include "PlayerAttackState.h"

#include "Actors/Player/Player.h"
#include "Weapons/Melee/MeleeWeapon.h"

#include "Input/InputManager.h"
#include "Input/Cursor.h"

#include "Audio/AudioManager.h"



PlayerAttackState::PlayerAttackState(Player* player)
{
	mWeapon = player->weapon();
}


void PlayerAttackState::init()
{
	mWeapon->attack();

	AudioManager::Get()->pushEvent(AudioEvent(AudioEvent::Play, mWeapon->missSoundLabel(), mWeapon));
}


void PlayerAttackState::fastUpdate(float dt)
{
	mWeapon->fastUpdate(dt);
}


void PlayerAttackState::slowUpdate(float dt)
{
	mWeapon->slowUpdate(dt);

	AudioManager* audio = AudioManager::Get();
	if (mWeapon->getCollider()->didHit() && !audio->isPlaying(mWeapon->hitSoundLabel(), mWeapon))
	{
		AudioManager::Get()->pushEvent(AudioEvent(AudioEvent::Stop, mWeapon->missSoundLabel(), mWeapon));
		AudioManager::Get()->pushEvent(AudioEvent(AudioEvent::Play, mWeapon->hitSoundLabel(), mWeapon));
	}
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