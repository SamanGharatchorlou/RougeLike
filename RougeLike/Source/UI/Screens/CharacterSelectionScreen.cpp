#include "pch.h"
#include "CharacterSelectionScreen.h"

#include "Game/GameData.h"
#include "UI/UIManager.h"
#include  "Graphics/TextureManager.h"

#include "UI/Elements/UIButton.h"


CharacterSelectionScreen::CharacterSelectionScreen(GameData* gameData) :
	Screen(gameData), 
	mEnterGame(false)
{

}

void CharacterSelectionScreen::enter()
{
	UIBox* character = static_cast<UIBox*>(mGameData->uiManager->findElement("Character"));

	BasicString str = mGameData->textureManager->getTextureName(character->texture());
	mSelectedCharacter.move(str);// = str;//.copy(*str);

	Texture* texture = mGameData->textureManager->getTexture(mSelectedCharacter + "Icon", FileManager::Image_UI);
	character->setTexture(texture);
}


void CharacterSelectionScreen::update(float dt)
{
	UIButton* button = mGameData->uiManager->findButton("PlayButton");

	if (button)
	{
		if (button->isReleased())
		{
			UIBox* weapon = static_cast<UIBox*>(mGameData->uiManager->findElement("Weapon"));
			const Texture* weaponTexture = weapon->texture();

			BasicString str = mGameData->textureManager->getTextureName(weaponTexture);
			mSelectedWeapon = str; //.copy(*str);
			mEnterGame = true;
		}

		UIBox* boxComponent = static_cast<UIBox*>(mGameData->uiManager->findElement("CharacterNamePanel"));

		if (boxComponent)
		{
			Texture* texture;

			if (button->isHeld())
			{
				texture = mGameData->textureManager->getTexture("Big button Pressed", FileManager::Image_UI);
			}
			else
			{
				texture = mGameData->textureManager->getTexture("Big button Released", FileManager::Image_UI);
			}

			boxComponent->setTexture(texture);
		}
	}
}


