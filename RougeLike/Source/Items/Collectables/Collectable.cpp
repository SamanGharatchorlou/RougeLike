#include "pch.h"
#include "Collectable.h"

#include "Graphics/Texture.h"

#include "Characters/Player/PlayerManager.h"
#include "Characters/Player/Player.h"


void Collectable::init(std::string value, Texture* texture, RectF rect)
{
	mValue = value;
	mTexture = texture;
	mRect = rect;

	mCollider.init(&mRect);
}


void Collectable::render()
{
	mTexture->render(mRect);
}



// --- Weapon pickup --- //
WeaponCollectable::WeaponCollectable(std::string weaponName, Texture* texture)
{
	RectF rect(VectorF(), texture->originalDimentions * 4.0f);

	init(weaponName, texture, rect);
}


void WeaponCollectable::activate(PlayerManager* playerManager)
{
	playerManager->selectWeapon(mValue);
}



// --- Health Pickup --- //
void HealthCollectable::activate(PlayerManager* playerManager)
{
	float health = std::stof(mValue);

	playerManager->get()->propertyBag().pHealth.get().increase(health);
}