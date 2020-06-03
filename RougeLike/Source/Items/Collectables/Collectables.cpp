#include "pch.h"
#include "Collectables.h"

#include "Collectable.h"

#include "Game/Camera.h"
#include "Game/GameData.h"
#include "Collisions/CollisionManager.h"

#include "Items/Spawner.h"
#include "Map/Environment.h"
#include "Graphics/TextureManager.h"

#include"Objects/Actors/ActorManager.h"
#include "Objects/Actors/Player/Player.h"

#if _DEBUG
#include "Debug/DebugDraw.h"
#endif


Collectables::Collectables(GameData* gameData) : mGameData(gameData) 
{ 
	timer.start();
}


void Collectables::slowUpdate(float dt)
{
	float oscillation = std::sin(timer.getSeconds() * dt * 500);
	VectorF oscillationVector = VectorF(oscillation, oscillation);

	std::vector<Collectable*>::iterator iter;
	for (iter = mCollectables.begin(); iter != mCollectables.end(); )
	{
		Collectable* collectable = *iter;

		collectable->move(oscillationVector);

		if (collectable->pickedUp())
		{
			collectable->activate(mGameData->actors->player());

			// Destroy the collectable from game
			iter = mCollectables.erase(iter);
			mGameData->collisionManager->removeDefender(CollisionManager::Player_Hit_Collectable, collectable->collider());
			delete collectable;
		}
		else
		{
			iter++;
		}
	}
}



void Collectables::render()
{
	for (unsigned int i = 0; i < mCollectables.size(); i++)
	{
		RectF worldRect = mCollectables[i]->rect();

		if (Camera::Get()->inView(worldRect))
		{
			RectF cameraRect = Camera::Get()->toCameraCoords(worldRect);
#if DRAW_COLLECTABLE_RECT
			debugDrawRect(mCollectables[i]->colliderRect(), RenderColour::Blue);
			debugDrawRect(worldRect, RenderColour::Green);
#else
			mCollectables[i]->render(cameraRect);
#endif
		}
	}
}



void Collectables::spawnRandomItem(ItemType itemType)
{
	for (int i = 0; i < 5; i++)
	{
		Spawner itemSpawner;
		int randomXPosition = 10; // randomNumberBetween(10, 95);
		VectorF position = itemSpawner.findSpawnPoint(mGameData->environment->primaryMap(), randomXPosition);

		std::vector<std::string> itemNameList = itemNames(itemType);
		const std::string weaponName = itemNameList[randomNumberBetween(0, itemNameList.size())];

		WeaponCollectable* weaponPickup = new WeaponCollectable(weaponName, mGameData->textureManager->getTexture(weaponName, FileManager::Image_Weapons));
		spawn(weaponPickup, position);
	}
}

void Collectables::spawn(Collectable* collectable, float xPosition)
{
	Spawner itemSpawner;
	VectorF position = itemSpawner.findSpawnPoint(mGameData->environment->primaryMap(), xPosition);

	collectable->setPosition(position);
	mCollectables.push_back(collectable);

	std::vector<Collider*> collider{ collectable->collider() };
	mGameData->collisionManager->addDefenders(CollisionManager::Player_Hit_Collectable, collider);
}


// Collectables now owns this object, must be destroyed.
void Collectables::spawn(Collectable* collectable, VectorF position)
{
	collectable->setPosition(position);
	mCollectables.push_back(collectable);

	std::vector<Collider*> collider{ collectable->collider() };
	mGameData->collisionManager->addDefenders(CollisionManager::Player_Hit_Collectable, collider);
}


/// --- Private Functions ---///
std::vector<std::string> Collectables::itemNames(ItemType type)
{
	switch (type)
	{
	case Collectables::MeleeWeapon:
		return FileManager::Get()->fileNamesInFolder(FileManager::Config_MeleeWeapons);
	default:
		DebugPrint(Log, "Unrecognised ItemType %d, cannot get item name list for item spawn\n", type);
		break;
	}
}