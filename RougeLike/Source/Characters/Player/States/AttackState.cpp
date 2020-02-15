#include "pch.h"
#include "AttackState.h"

#include "Game/GameData.h"
#include "Characters/Weapons/Weapon.h"
#include "Characters/Player/Player.h"
#include "Collisions/Collider.h"


AttackState::AttackState(GameData* gameData, Player* player)
{
	setupStateData(gameData, player);
	mState = PlayerState::Attack;

	mWeapon = &mPlayer->getWeapon();
	mWeapon->overrideCursorControl(true);
	mWeapon->setColliderActivite(true);

	mRotationSum = 0.0;
}


void AttackState::fastUpdate(float dt)
{
	// weapon has completed its attack rotation
	if (mRotationSum > mWeapon->getMaxSwingAngle())
	{
		mPlayer->popState();
	}
	else
	{
		double theta = mWeapon->getSwingSpeed() * dt;
		mRotationSum += theta;

		if (mRotationSum > mWeapon->getMaxSwingAngle())
			theta = mWeapon->getMaxSwingAngle() - mRotationSum;

		mWeapon->rotate(theta);
	}
}


void AttackState::exit()
{
	mWeapon->flipSide();
	mWeapon->overrideCursorControl(false);
	mWeapon->setColliderActivite(false);
}