#include "pch.h"
#include "MagicWeapon.h"

#include "MagicProjectile.h"
#include "Game/Camera/Camera.h"

#if DEBUG_CHECK
#include "Debug/DebugDraw.h"
#endif

constexpr int poolSize = 100;


void MagicWeapon::equipt(const WeaponData* data)
{
	mData.copy(data);

	VectorF size = VectorF(mData.maxDimention, mData.maxDimention);
	mRect.SetSize(size);

	const MagicWeaponData* magicData = static_cast<const MagicWeaponData*>(data);

	// Build a bunch of projectiles and stuff them into a pool
	mPoolHead = new MagicProjectile[poolSize];
	for (int i = 0; i < poolSize; i++)
	{
		MagicProjectile* projectile = mPoolHead + i;
		projectile->init(magicData);
		mProjectilePool.push(projectile);
	}
}


void MagicWeapon::unequipt()
{
	for (Queue<MagicProjectile*>::iterator iter = mProjectilePool.begin(); iter != mProjectilePool.end(); iter++)
	{
		delete[] mPoolHead;
	}

	mProjectilePool.clear();
	mActiveProjectiles.clear();
	mPoolHead = nullptr;
}




void MagicWeapon::attack()
{
	mAttacking = true;

	if (mProjectilePool.size() > 0)
	{
		MagicProjectile* projectile = mProjectilePool.pop();
		projectile->fire(rect().Center(), mDirection);
		mActiveProjectiles.push(projectile);
	}
	else
	{
		DebugPrint(Warning, "Projectile pool is empty! consider increasing the size\n");
	}
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


std::vector<Collider*> MagicWeapon::getColliders()
{
	std::vector<Collider*> colliders;

	for (Queue<MagicProjectile*>::iterator iter = mActiveProjectiles.begin(); iter != mActiveProjectiles.end(); iter++)
	{
		MagicProjectile* projectile = *iter;
		if (!projectile->mDeactivated)
			colliders.push_back(&projectile->mCollider);
	}

	return colliders;
}


void MagicWeapon::fastUpdate(float dt)
{
	for (Queue<MagicProjectile*>::iterator iter = mActiveProjectiles.begin(); iter != mActiveProjectiles.end(); iter++)
	{
		MagicProjectile* projectile = *iter;
		if (!projectile->mDeactivated)
		{
			// If a projectile hits anything, deactivate it, its removal is handled in slowUpdate
			if (projectile->mCollider.didHit())
				projectile->mDeactivated = true;
			else
				projectile->move(dt);
		}
	}
}


void MagicWeapon::slowUpdate(float dt)
{
	for (Queue<MagicProjectile*>::iterator iter = mActiveProjectiles.begin(); iter != mActiveProjectiles.end(); iter++)
	{
		MagicProjectile* projectile = *iter;
		if (!projectile->mDeactivated)
			projectile->mAnimator.slowUpdate(dt);
	}

	// Only ever pop the front of the queue, this maintains a cache friendly queue
	// e.g. projectile1 missed and travels for a distance, while projectil2 hits right away.
	while (mActiveProjectiles.size() > 0 && mActiveProjectiles.front()->mCollider.didHit())
	{
		MagicProjectile* projectile = mActiveProjectiles.pop();

		projectile->reset();
		mProjectilePool.push(projectile);
	}
}


void MagicWeapon::render()
{
#if TRACK_COLLISIONS
	mCollider.renderCollider();
#endif
#if DRAW_PLAYER_WEAPON_RECT
	VectorF about = rect().TopCenter() + VectorF(0.0f, rect().Height() * 0.85f);
	debugDrawPoint(about, 10.0f, RenderColour::Black);

#if !TRACK_COLLISIONS
	debugDrawRect(rect(), RenderColour::Blue);
#endif
#endif

	for (Queue<MagicProjectile*>::iterator iter = mActiveProjectiles.begin(); iter != mActiveProjectiles.end(); iter++)
	{
		MagicProjectile* projectile = *iter;
		if (!projectile->mDeactivated)
			projectile->render();
	}
}


void MagicWeapon::updateAimDirection(VectorF cursorPosition)
{
	VectorF playerPosition = rect().Center();
	mDirection = (cursorPosition - Camera::Get()->toCameraCoords(playerPosition)).normalise();
}


// Audio
const BasicString& MagicWeapon::missSoundLabel() { return mData.audioMiss(); };
const BasicString& MagicWeapon::hitSoundLabel() { return mData.audioHit(); };