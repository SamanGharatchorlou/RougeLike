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

void Collectable::set(const std::string& id)
{
	mID = id;
}


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
void WeaponCollectable::activate(Player* Player)
{
	//Player->selectWeapon(mName);
}


// --- Ability pickup --- //
void AbilityCollectable::activate(Player* Player)
{
	Player->addAbility(mID);
}




