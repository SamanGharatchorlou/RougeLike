#include "pch.h"
#include "Collectable.h"

#include "Graphics/Texture.h"

#include "Objects/Actors/Player/Player.h"

#include "Objects/Abilities/Ability.h"


Collectable::Collectable() : mIcon(nullptr) 
{
	mCollider.init(&mRect);

#if _DEBUG
	mCollider.setName("Collectable");
#endif
};


void Collectable::setIcon(Texture* icon)
{
	VectorF position;
	VectorF maxDimentions(50.0f, 50.0f);
	VectorF iconSize = icon->originalDimentions;
	float ratio = 1.0f;

	if (iconSize.y > iconSize.x)
		ratio = iconSize.y / maxDimentions.y;
	else
		ratio = iconSize.x / maxDimentions.x;

	mRect = RectF(position, iconSize / ratio);
	mIcon = icon;
}


void Collectable::render(RectF cameraRect) const
{
	mIcon->render(cameraRect);
}


// --- Weapon pickup --- //
WeaponCollectable::WeaponCollectable(const std::string& name)
{
	mName = name;
}


AbilityCollectable::AbilityCollectable(const std::string& name)
{
	Ability* ability = createNewAbility(name);
	ASSERT(Warning, ability != nullptr, "the ability '%s' was not found by the finder\n", name);

	mAbility = ability;
	mName = name; // TODO: use the ability->name() instead?
}


void AbilityCollectable::activate(Player* Player)
{
	Player->addAbility(mAbility);
}


void WeaponCollectable::activate(Player* Player)
{
	Player->selectWeapon(mName);
}


//
//AbilityCollectable::AbilityCollectable(const std::string& ability, Texture* texture)
//{
//
//}
//
//
//
//
//// --- Health Pickup --- //
//void HealthCollectable::activate(Player* Player)
//{
//	int health = std::stoi(mValue);
////	Player->get()->propertyBag()->pHealth.get().increase(health);
//}