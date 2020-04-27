#include "pch.h"
#include "Collectable.h"

#include "Graphics/Texture.h"

#include "Characters/Player/PlayerManager.h"
#include "Characters/Player/Player.h"


void Collectable::init(const std::string& value, Texture* texture, RectF rect)
{
	mValue = value;
	mTexture = texture;
	mRect = rect;

	mCollider.init(&mRect, VectorF(2.5f, 1.5f));
}


void Collectable::render(RectF rect) const
{
	mTexture->render(rect);
}



// --- Weapon pickup --- //
WeaponCollectable::WeaponCollectable(const std::string& weaponName, Texture* texture)
{
	RectF rect(VectorF(), texture->originalDimentions);

	init(weaponName, texture, rect);
}


void WeaponCollectable::activate(PlayerManager* playerManager)
{
	playerManager->selectWeapon(mValue);
}



// --- Health Pickup --- //
void HealthCollectable::activate(PlayerManager* playerManager)
{
	int health = std::stoi(mValue);

	playerManager->get()->propertyBag()->pHealth.get().increase(health);
}