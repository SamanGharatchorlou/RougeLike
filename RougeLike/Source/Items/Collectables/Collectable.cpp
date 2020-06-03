#include "pch.h"
#include "Collectable.h"

#include "Graphics/Texture.h"

#include "Objects/Actors/Player/Player.h"

#include "Objects/Abilities/AbilityFinder.h"


Collectable::Collectable(Texture* icon) : mIcon(icon)
{
	setIcon(icon);
	mCollider.init(&mRect, VectorF(1.25f, 1.25f));

#if _DEBUG
	mCollider.setName("Collectable");
#endif
}

void Collectable::setIcon(Texture* icon)
{
	VectorF position;

	VectorF maxDimentions(50.0f, 50.0f);
	VectorF iconSize = icon->originalDimentions;

	float ratio = 1.0f;

	if (iconSize.y > iconSize.x)
	{
		ratio = iconSize.y / maxDimentions.y;
	}
	else
	{
		ratio = iconSize.x / maxDimentions.x;
	}

	mRect = RectF(position, iconSize / ratio);
}


void Collectable::render(RectF cameraRect) const
{
	mIcon->render(cameraRect);
}


// --- Weapon pickup --- //
WeaponCollectable::WeaponCollectable(const std::string& name, Texture* icon) : Collectable(icon), mName(name)
{

}


AbilityCollectable::AbilityCollectable(const std::string& name, Texture* icon) : Collectable(icon)
{
	AbilityFinder finder;
	Ability* ability = finder.get(name);

	ASSERT(Warning, ability != nullptr, "the ability '%s' was not found by the finder\n", name);

	mAbility = ability;
}


void AbilityCollectable::activate(Player* Player)
{
	Player->addAbility("Armor", mAbility);
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