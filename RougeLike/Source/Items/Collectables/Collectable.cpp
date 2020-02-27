#include "pch.h"
#include "Collectable.h"

#include "Graphics/Texture.h"
#include "Characters/Player/PlayerManager.h"


void Collectable::init(Texture* texture, RectF rect, std::string id)
{
	mTexture = texture;
	mRect = rect;
	mId = id;

	mCollider.init(&mRect);
}


void Collectable::render()
{
	mTexture->render(mRect);
}



// -- Weapon pickup -- //
void WeaponCollectable::activate(PlayerManager* playerManager)
{
	playerManager->selectWeapon(mId);
}