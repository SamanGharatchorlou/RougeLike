#include "pch.h"
#include "RangedWeapon.h"

#include "Projectile.h"
#include "Weapons/WeaponData.h"

#include "Game/GameData.h"
#include "Game/Camera.h"
#include "Graphics/Texture.h"

#include "Collisions/DamageCollider.h"

#include "Objects/Actors/Player/PlayerPropertyBag.h"

#include "Map/Map.h"

constexpr int quiverStashCount = 100;


RangedWeapon::RangedWeapon() : quiver(quiverStashCount)
{

}


RangedWeapon::~RangedWeapon()
{
}


void RangedWeapon::attack()
{
	Projectile* projectile = quiver.draw();

	if (projectile)
	{
		projectile->fire(mRect.Center(), mDirection);

		travelingProjectiles.push_back(projectile);
	}
	else
	{
		quiver.load();
	}
}


void RangedWeapon::fastUpdate(float dt)
{
	for (std::list<Projectile*>::iterator iter = travelingProjectiles.begin(); iter != travelingProjectiles.end(); iter++)
	{
		(*iter)->move(dt);
	}
}

void RangedWeapon::slowUpdate(float dt)
{
	// NOTE: The GameInfo object was removed, can this be implemented without it? (date: 06/05/2020)
	//for (std::list<Projectile*>::iterator iter = travelingProjectiles.begin(); iter != travelingProjectiles.end(); iter++)
	//{
	//	VectorF position = (*iter)->position();

	//	
	//	// Lost projectil from wall collision
	//	if (GameInfo::Get()->isWall(position))
	//	{
	//		quiver.lostProjectile(*iter);
	//		travelingProjectiles.erase(iter);
	//		break;
	//	}

	//	// Lost projectile from out of bounds
	//	VectorF mapBoundaries = GameInfo::Get()->mapDimentions();

	//	bool outOfBounds =
	//		position.x < 0.0f || position.x > mapBoundaries.x ||
	//		position.y < 0.0f || position.y > mapBoundaries.y;

	//	if (outOfBounds)
	//	{
	//		quiver.lostProjectile(*iter);
	//		travelingProjectiles.erase(iter);
	//		break;
	//	}

	//	if ((*iter)->hasCollided())
	//	{
	//		quiver.lostProjectile(*iter);
	//		travelingProjectiles.erase(iter);
	//		break;
	//	}
	//}
}

void RangedWeapon::equipt(const WeaponData* data)
{
	mData = static_cast<const RangedWeaponData*>(data); 
	
	quiver.init(mData);

	// TODO: where/what to do with this 1.5 scale factor
	mRect.SetSize(mData->texture->originalDimentions * 1.5f);
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

	for (std::list<Projectile*>::iterator iter = travelingProjectiles.begin(); iter != travelingProjectiles.end(); iter++)
	{
		if (Camera::Get()->inView((*iter)->rect()))
		{
			RectF rect = Camera::Get()->toCameraCoords((*iter)->rect());
			(*iter)->render(rect);
		}
	}
}


const std::vector<Collider*> RangedWeapon::getColliders()
{
	std::vector<Collider*> colliders;

	for (std::list<Projectile*>::iterator iter = travelingProjectiles.begin(); iter != travelingProjectiles.end(); iter++)
	{
		colliders.push_back((*iter)->collider());
	}

	return colliders;
}

