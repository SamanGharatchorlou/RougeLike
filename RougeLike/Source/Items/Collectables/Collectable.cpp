#include "pch.h"
#include "Collectable.h"

#include "Graphics/Texture.h"

#include "Objects/Actors/Player/Player.h"


void Collectable::init(const std::string& value, Texture* texture, RectF rect)
{
	mValue = value;
	mTexture = texture;
	mRect = rect;

	mCollider.init(&mRect, VectorF(2.5f, 1.5f));

#if _DEBUG
	mCollider.setName(value + "_collectable");
#endif
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


void WeaponCollectable::activate(Player* Player)
{
	Player->selectWeapon(mValue);
}



// --- Health Pickup --- //
void HealthCollectable::activate(Player* Player)
{
	int health = std::stoi(mValue);
//	Player->get()->propertyBag()->pHealth.get().increase(health);
}