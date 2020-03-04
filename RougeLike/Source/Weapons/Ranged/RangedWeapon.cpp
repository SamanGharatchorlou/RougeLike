#include "pch.h"
#include "RangedWeapon.h"

#include "Projectile.h"
#include "Weapons/WeaponData.h"

#include "Game/Cursor.h"
#include "Game/Camera.h"
#include "Graphics/Texture.h"

#include "Collisions/DamageCollider.h"

#include "Characters/Player/PlayerPropertyBag.h"

#include "Game/Camera.h"

RangedWeapon::RangedWeapon()
{

}


RangedWeapon::~RangedWeapon()
{
}


void RangedWeapon::attack()
{
	Projectile* projectile = new Projectile(mData);

	VectorF camPos = Camera::Get()->toCameraCoords(mRect.Center());

	projectile->fire(camPos, mDirection);

	projectiles.push_back(projectile);
}


void RangedWeapon::fastUpdate(float dt)
{
	for (unsigned int i = 0; i < projectiles.size(); i++)
	{
		projectiles[i]->move(dt);
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

	for (unsigned int i = 0; i < projectiles.size(); i++)
	{
		projectiles[i]->render();
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
