#include "pch.h"
#include "CharacterSelectionScreen.h"

#include  "Graphics/TextureManager.h"
#include "UI/UIManager.h"


CharacterSelectionScreen::CharacterSelectionScreen(GameData* gameData) :
	Screen(gameData), 
	mEnterGame(false)
{

}


void CharacterSelectionScreen::update()
{
	UIButton* button = mGameData->uiManager->findButton("PlayButton");

	if (button)
	{
		if (button->isReleased())
		{
			mSelectedCharacter = "Soldier.xml";
			mEnterGame = true;
		}

		UIBox* boxComponent = static_cast<UIBox*>(mGameData->uiManager->findElement("CharacterNamePanel"));

		if (boxComponent)
		{
			Texture* texture;

			if (button->isHeld())
			{
				texture = mGameData->textureManager->getTexture("Big button Pressed");
			}
			else
			{
				texture = mGameData->textureManager->getTexture("Big button Released");
			}

			boxComponent->setTexture(texture);
		}
	}
}