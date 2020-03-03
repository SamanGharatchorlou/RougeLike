#include "pch.h"
#include "Collectable.h"

#include "Graphics/Texture.h"

#include "Characters/Player/PlayerManager.h"
#include "Characters/Player/Player.h"

#include "Game/Camera.h"


void Collectable::init(std::string value, Texture* texture, RectF rect)
{
	mValue = value;
	mTexture = texture;
	mRect = rect;

	mCollider.init(&mRect);
}


void Collectable::render(Camera* camera)
{
	RectF rect = camera->toWorldCoords(mRect);
	mTexture->render(rect);
}


void Collectable::toWorldCoords(Camera* camera)
{
	mRect = camera->toWorldCoords(mRect);
}



// --- Weapon pickup --- //
WeaponCollectable::WeaponCollectable(const std::string& weaponName, Texture* texture)
{
	RectF rect(VectorF(), texture->originalDimentions);

	init(weaponName, texture, rect);
}


void WeaponCollectable::activate(PlayerManager* playerManager)
{
	playerManager->selectWeapon("BigHammer");
}



// --- Health Pickup --- //
void HealthCollectable::activate(PlayerManager* playerManager)
{
	int health = std::stoi(mValue);

	playerManager->get()->propertyBag()->pHealth.get().increase(health);
}