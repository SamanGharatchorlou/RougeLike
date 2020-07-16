#include "pch.h"
#include "Collectable.h"

#include "Graphics/Texture.h"

#include "Actors/Player/Player.h"



Collectable::Collectable() : mIcon(nullptr) 
{
	mCollider.init(&mRect);

#if _DEBUG
	mCollider.setName("Collectable");
#endif
};


void Collectable::setIcon(Texture* icon)
{
	float maxDimention = 50.0f;
	VectorF iconSize = realiseSize(icon->originalDimentions, maxDimention);
	mRect.SetSize(realiseSize(iconSize, maxDimention));
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


// --- Ability pickup --- //
AbilityCollectable::AbilityCollectable(const std::string& ability)
{
	mName = ability;
}


void AbilityCollectable::activate(Player* Player)
{
	Player->addAbility(mName);
}


void WeaponCollectable::activate(Player* Player)
{
	Player->selectWeapon(mName);
}

