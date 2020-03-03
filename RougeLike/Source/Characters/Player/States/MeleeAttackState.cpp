#include "pch.h"
#include "MeleeAttackState.h"

#include "Game/GameData.h"
#include "Weapons/Weapon.h"
#include "Weapons/Melee/MeleeWeapon.h"

#include "Characters/Player/Player.h"
#include "Collisions/Collider.h"


MeleeAttackState::MeleeAttackState(GameData* gameData, Player* player) : 
	mRotationSum(0.0)
{
	setupStateData(gameData, player);
	mState = PlayerState::Attack;

	mWeapon = static_cast<MeleeWeapon*>(mPlayer->getWeapon());
	mWeapon->overrideCursorControl(true);
	mWeapon->setColliderActivite(true);
}


void MeleeAttackState::fastUpdate(float dt)
{
	// weapon has completed its attack rotation
	if (mRotationSum > mWeapon->maxSwingAngle())
	{
		mPlayer->popState();
	}
	else
	{
		double theta = mWeapon->swingSpeed() * dt;
		mRotationSum += theta;

		if (mRotationSum > mWeapon->maxSwingAngle())
			theta = mWeapon->maxSwingAngle() - mRotationSum;

		mWeapon->rotate(theta);
	}
}


void MeleeAttackState::exit()
{
	mWeapon->flipSide();
	mWeapon->overrideCursorControl(false);
	mWeapon->setColliderActivite(false);
}