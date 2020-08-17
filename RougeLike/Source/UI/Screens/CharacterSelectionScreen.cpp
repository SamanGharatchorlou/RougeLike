#include "pch.h"
#include "CharacterSelectionScreen.h"

#include "Game/Data/GameData.h"
#include "UI/UIManager.h"
#include  "Graphics/TextureManager.h"

#include "UI/Elements/UIButton.h"


CharacterSelectionScreen::CharacterSelectionScreen() : mEnterGame(false), mPlayButton(nullptr) { }


void CharacterSelectionScreen::enter()
{
	const TextureManager* textures = TextureManager::Get();
	UIBox* character = static_cast<UIBox*>(find("Character"));
	mSelectedCharacter = textures->getTextureName(character->texture());

	Texture* texture = textures->getTexture(mSelectedCharacter + "Icon", FileManager::Image_UI);
	character->setTexture(texture);

	mPlayButton = findButton("PlayButton");
}


void CharacterSelectionScreen::update(float dt)
{
	if (mPlayButton->isReleased())
	{
		selectWeapon();
		mEnterGame = true;
	}
}


void CharacterSelectionScreen::selectWeapon()
{
	//UIBox* weapon = static_cast<UIBox*>(find("Weapon"));
	//const Texture* weaponTexture = weapon->texture();

	//const TextureManager* textures = TextureManager::Get();
	mSelectedWeapon = "Sword";// textures->getTextureName("Sword");
}