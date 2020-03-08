#include "pch.h"
#include "RangedWeapon.h"

#include "Projectile.h"
#include "Weapons/WeaponData.h"

#include "Game/GameData.h"
#include "Game/Camera.h"
#include "Graphics/Texture.h"

#include "Collisions/DamageCollider.h"

#include "Characters/Player/PlayerPropertyBag.h"

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
	VectorF camPos = Camera::Get()->toCameraCoords(mRect.Center());

	Projectile* projectile = quiver.draw();

	if (projectile)
	{
		projectile->fire(camPos, mDirection);

		travelingProjectiles.push_back(projectile);
	}
	else
	{
		printf("loading\n");
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


// WARNING: requires testing
// will only processing the front projectile break things?
/*
void RangedWeapon::slowUpdate(float dt)
{
	if (travelingProjectiles.size() > 0)
	{
		for (unsigned int i = 0; i < travelingProjectiles.size(); i++)
		{
			Projectile* projectile = travelingProjectiles[i];

			if (!projectile->isActive())
			{
				if (i == 0)
				{

					travelingProjectiles.pop_front();
					quiver.lostProjectile();
					printf("fast popping front\n");
					return;
				}
				else
					continue;
			}


			// Lost projectile from collision
			bool remove = projectile->hasCollided();

			VectorF position = projectile->position();
			position = Camera::Get()->toWorldCoords(position);

			// Lost projectile from out of bounds
			if (!remove)
			{
				VectorF mapBoundaries = GameInfo::Get()->mapDimentions();

				bool outOfBounds =
					position.x < 0.0f				|| 
					position.x > mapBoundaries.x	||
					position.y < 0.0f				|| 
					position.y > mapBoundaries.y;

				remove = outOfBounds;

				if(remove)
					printf("out of bounds\n");
			}
	
			// Lost projectile from wall collision
			if (!remove)
			{
				remove = GameInfo::Get()->isWall(position);

				if(remove)
					printf("wall collision\n");
			}

			if (remove)
			{
				projectile->setActive(false);

				if (i == 0)
				{
					travelingProjectiles.pop_front();
					quiver.lostProjectile();
					printf("popping front\n");
				}
			}
		}
		
	}

} 
*/

void RangedWeapon::slowUpdate(float dt)
{
	//for (unsigned int i = 0; i < travelingProjectiles.size(); i++)
	//{
	//	VectorF position = travelingProjectiles[i]->position();
	//	position = Camera::Get()->toWorldCoords(position);

	//	//// Lost projectile from out of bounds
	//	//VectorF mapBoundaries = GameInfo::Get()->mapDimentions();

	//	//bool outOfBounds =
	//	//	position.x < 0.0f ||
	//	//	position.x > mapBoundaries.x ||
	//	//	position.y < 0.0f ||
	//	//	position.y > mapBoundaries.y;

	//	//if (remove)
	//	//	printf("out of bounds\n");
	//	//}

	//	// Lost projectile from wall collision
	//	if (GameInfo::Get()->isWall(position))
	//	{
	//		quiver.lostProjectile(travelingProjectiles[i]);
	//	}
	//}

	for (std::list<Projectile*>::iterator iter = travelingProjectiles.begin(); iter != travelingProjectiles.end(); iter++)
	{
		VectorF position = (*iter)->position();
		position = Camera::Get()->toWorldCoords(position);

		if (GameInfo::Get()->isWall(position))
		{
			quiver.lostProjectile(*iter);
			travelingProjectiles.erase(iter);
			// TODO: why does this not work?????
			// this return fucks it??
			return;
		}
	}
}

void RangedWeapon::equipt(const WeaponData* data)
{
	mData = static_cast<const RangedWeaponData*>(data); 
	
	quiver.init(mData);

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

	for (std::list<Projectile*>::iterator iter = travelingProjectiles.begin(); iter != travelingProjectiles.end(); iter++)
	{
		(*iter)->render();
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

