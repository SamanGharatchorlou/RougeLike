#include "pch.h"
#include "CharacterSelectionScreen.h"

#include "Game/Data/GameData.h"
#include "UI/UIManager.h"
#include  "Graphics/TextureManager.h"

#include "UI/Elements/UIButton.h"


CharacterSelectionScreen::CharacterSelectionScreen(const TextureManager* textures) : Screen(textures), mEnterGame(false), mPlayButton(nullptr) { }


void CharacterSelectionScreen::enter()
{
	UIBox* character = static_cast<UIBox*>(find("Character"));
	mSelectedCharacter = mTextures->getTextureName(character->texture());

	Texture* texture = mTextures->getTexture(mSelectedCharacter + "Icon", FileManager::Image_UI);
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

	//Button::State playButtonState = mPlayButton->

	//if (mPlayButton->isPressed())
	//{
	//	UIBox* boxComponent = static_cast<UIBox*>(find("CharacterNamePanel"));
	//	if (boxComponent)
	//	{
	//		BasicString textureId = mPlayButton->isHeld() ? "Big button Pressed" : "Big button Released";
	//		Texture* texture = mTextures->getTexture(textureId, FileManager::Image_UI);
	//		boxComponent->setTexture(texture);
	//		printf("setting");
	//	}
	//}
}


void CharacterSelectionScreen::selectWeapon()
{
	UIBox* weapon = static_cast<UIBox*>(find("Weapon"));
	const Texture* weaponTexture = weapon->texture();

	mSelectedWeapon = mTextures->getTextureName(weaponTexture);
}