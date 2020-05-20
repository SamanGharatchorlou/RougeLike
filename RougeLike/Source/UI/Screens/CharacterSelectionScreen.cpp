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


void CharacterSelectionScreen::update(float dt)
{
	UIButton* button = mGameData->uiManager->findButton("PlayButton");

	if (button)
	{
		if (button->isReleased())
		{
			UIBox* character = static_cast<UIBox*>(mGameData->uiManager->findElement("Character"));
			const Texture* characterTexture = character->texture();

			UIBox* weapon = static_cast<UIBox*>(mGameData->uiManager->findElement("Weapon"));
			const Texture* weaponTexture = weapon->texture();


			mSelectedCharacter = mGameData->textureManager->getTextureName(characterTexture);
			mSelectedWeapon = mGameData->textureManager->getTextureName(weaponTexture);
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


