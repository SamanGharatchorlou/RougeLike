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

#include "Objects/Abilities/Ability.h"

#if _DEBUG
#include "Debug/DebugDraw.h"
#endif


Collectables::Collectables(GameData* gameData) : mGameData(gameData) 
{ 
	timer.start();
}


void Collectables::slowUpdate(float dt)
{
	float theta = timer.getSeconds() * 2.0f;
	float xOscillation = std::sin(theta);
	float yOscillation = std::cos(theta * 2.0f);
	VectorF oscillationVector = VectorF(xOscillation, yOscillation) / 2.0f;

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
#endif
			mCollectables[i]->render(cameraRect);
		}
	}
}


void Collectables::spawn(Collectable* collectable, float xPosition)
{
	Spawner itemSpawner;
	VectorF position = itemSpawner.findSpawnPoint(mGameData->environment->primaryMap(), xPosition);

	collectable->setIcon(findIcon(collectable));
	collectable->setPosition(position);
	mCollectables.push_back(collectable);

	std::vector<Collider*> collider{ collectable->collider() };
	mGameData->collisionManager->addDefenders(CollisionManager::Player_Hit_Collectable, collider);
}


//void Collectables::spawnRandomItem(ItemType itemType)
//{
//	for (int i = 0; i < 5; i++)
//	{
//		Spawner itemSpawner;
//		int randomXPosition = 10; // randomNumberBetween(10, 95);
//		VectorF position = itemSpawner.findSpawnPoint(mGameData->environment->primaryMap(), randomXPosition);
//
//		std::vector<std::string> itemNameList = itemNames(itemType);
//		const std::string weaponName = itemNameList[randomNumberBetween(0, itemNameList.size())];
//
//		WeaponCollectable* weaponPickup = new WeaponCollectable(weaponName, mGameData->textureManager->getTexture(weaponName, FileManager::Image_Weapons));
//		spawn(weaponPickup, position);
//	}
//}

/// --- Private Functions ---///
//std::vector<std::string> Collectables::itemNames(ItemType type)
//{
//	switch (type)
//	{
//	case ItemType::MeleeWeapon:
//		return FileManager::Get()->fileNamesInFolder(FileManager::Config_MeleeWeapons);
//	default:
//		DebugPrint(Log, "Unrecognised ItemType %d, cannot get item name list for item spawn\n", type);
//		break;
//	}
//}

Texture* Collectables::findIcon(Collectable* collectable) const
{
	ValueMap info = getConfigInfo(collectable);
	Texture* texture = mGameData->textureManager->getTexture(info["Icon"], FileManager::Image_UI);

	ASSERT(Warning, texture != nullptr, "Collectable '%s' info has no valid icon name\n", collectable->name().c_str());

	return texture;
}

ValueMap Collectables::getConfigInfo(Collectable* collectable) const
{
	XMLParser parser;
	std::string filePath = FileManager::Get()->findFile(FileManager::Configs_Objects, collectable->name());
	parser.parseXML(filePath);
	return parser.values(parser.rootNode());
}