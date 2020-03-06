#include "pch.h"
#include "RangedWeapon.h"

#include "Projectile.h"
#include "Weapons/WeaponData.h"

#include "Game/Camera.h"
#include "Graphics/Texture.h"

#include "Collisions/DamageCollider.h"

#include "Characters/Player/PlayerPropertyBag.h"

constexpr int quiverStashCount = 30;


RangedWeapon::RangedWeapon() : quiver(quiverStashCount)
{

}


RangedWeapon::~RangedWeapon()
{
}


void RangedWeapon::attack()
{
	VectorF camPos = Camera::Get()->toCameraCoords(mRect.Center());

	Projectile* projectile = quiver.draw();

	projectile->fire(camPos, mDirection);

	travelingProjectiles.push_back(projectile);
}


void RangedWeapon::fastUpdate(float dt)
{
	for (unsigned int i = 0; i < travelingProjectiles.size(); i++)
	{
		travelingProjectiles[i]->move(dt);
	}
}


void RangedWeapon::slowUpdate(float dt)
{
	if (travelingProjectiles.front()->hasCollided())
	{
		travelingProjectiles.pop_front();
		quiver.lostProjectile();
	}
}


void RangedWeapon::equipt(const WeaponData* data)
{
	mData = static_cast<const RangedWeaponData*>(data);

	// TODO: where/what to do with this 1.5 scale factor
	mRect.SetSize(mData->texture->originalDimentions * 1.5f);
}


void RangedWeapon::updateStats(const PlayerPropertyBag* bag)
{

}


// Follow character
void RangedWeapon::updateAnchor(VectorF anchor)
{
	mRect.SetTopLeft(anchor + mData->handleOffset);
}


void RangedWeapon::updateAimDirection(VectorF cursorPosition)
{
	// Follow cursor
	if (!mOverrideCursorControl)
	{
		// Camera to cursor vector
		mDirection = (cursorPosition - Camera::Get()->toCameraCoords(mRect.BotCenter()));
	}
}


void RangedWeapon::render()
{
	mData->texture->render(Camera::Get()->toCameraCoords(mRect));

	for (unsigned int i = 0; i < travelingProjectiles.size(); i++)
	{
		travelingProjectiles[i]->render();
	}
}




const std::vector<Collider*> RangedWeapon::getColliders() const
{
	std::vector<Collider*> colliders;

	// TODO: get all traveling projectile colliders
	//for (unsigned int i = 0; i < mBlockColliders.size(); i++)
	//{
	//	colliders.push_back(mBlockColliders[i]);
	//}

	return colliders;
}


// TODO: maybe dont need this?
void RangedWeapon::setColliderActivite(bool isActive)
{
	//for (unsigned int i = 0; i < mBlockColliders.size(); i++)
	//{
	//	mBlockColliders[i]->setActive(isActive);
	//}
}
