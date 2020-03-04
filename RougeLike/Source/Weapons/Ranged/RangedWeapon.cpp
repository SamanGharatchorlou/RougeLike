#include "pch.h"
#include "RangedWeapon.h"

#include "Weapons/WeaponData.h"

#include "Game/Cursor.h"
#include "Game/Camera.h"
#include "Graphics/Texture.h"

#include "Collisions/DamageCollider.h"

#include "Characters/Player/PlayerPropertyBag.h"


RangedWeapon::RangedWeapon()
{

}


RangedWeapon::~RangedWeapon()
{
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


void RangedWeapon::updateAimDirection(Camera* camera, VectorF cursorPosition)
{
	// Follow cursor
	if (!mOverrideCursorControl)
	{
		// Camera to cursor vector
		mDirection = (cursorPosition - camera->toCameraCoords(mRect.BotCenter()));
	}
}


void RangedWeapon::render(Camera* camera)
{
	mData->texture->render(camera->toCameraCoords(mRect));
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