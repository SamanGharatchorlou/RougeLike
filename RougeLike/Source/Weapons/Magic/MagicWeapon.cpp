#include "pch.h"
#include "MagicWeapon.h"

#include "Game/Camera/Camera.h"

void MagicWeapon::equipt(const WeaponData* data)
{
	mData.copy(data);

	VectorF size = VectorF(mData.maxDimention, mData.maxDimention);
	mRect.SetSize(size);

	const MagicWeaponData* magicData = static_cast<const MagicWeaponData*>(data);

	for (int i = 0; i < 100; i++)
	{
		MagicProjectile projectile(magicData);
		mProjectilePool.push(projectile);
	}
}

void MagicWeapon::attack()
{
	mAttacking = true;

	MagicProjectile& projectile = mProjectilePool.pop();
	projectile.fire(rect().Center(), mDirection);
	mActiveProjectiles.push(projectile);
}


std::vector<Effect*> MagicWeapon::getEffects(EffectPool* effectPool)
{
	std::vector<Effect*> effects;

	//mMeleeData.effectData.addXYPosition(mRect.Center());

	//effects.push_back(effectPool->getObject(EffectType::Damage));
	//effects.push_back(effectPool->getObject(EffectType::Displacement));

	//for (Effect* effect : effects)
	//{
	//	effect->fill(mMeleeData.effectData);
	//}

	return effects;
}


void MagicWeapon::fastUpdate(float dt)
{
	for (Queue<MagicProjectile>::iterator iter = mActiveProjectiles.begin(); iter != mActiveProjectiles.end(); iter++)
	{
		iter->move(dt);
	}
}


void MagicWeapon::render()
{
	for (Queue<MagicProjectile>::iterator iter = mActiveProjectiles.begin(); iter != mActiveProjectiles.end(); iter++)
	{
		iter->render();
	}
}


void MagicWeapon::updateAimDirection(VectorF cursorPosition)
{
	VectorF playerPosition = rect().Center();
	mDirection = (cursorPosition - Camera::Get()->toCameraCoords(playerPosition));
}


// Audio
const BasicString& MagicWeapon::missSoundLabel() { return mData.audioMiss(); };
const BasicString& MagicWeapon::hitSoundLabel() { return mData.audioHit(); };