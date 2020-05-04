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
	while (!mActions.empty())
	{
		switch (mActions.front())
		{
		case UIButton::SelectSoldier_4:
			mSelectedCharacter = "Soldier.xml";
			mEnterGame = true;

			break;
		default:
			break;
		}

		mActions.pop();
	}

	// textureSelected = "Big button Pressed"

	UIElement* element = mGameData->uiManager->find("PlayButton");

	if (element)
	{
		UIButton* button = static_cast<UIButton*>(element);
		if (button->isPressed())
		{
			UIElement* buttonPanel = mGameData->uiManager->find("CharacterNamePanel");

			UIButton* button = static_cast<UIButton*>(buttonPanel);
			if (button)
			{
				Texture* texture = mGameData->textureManager->getTexture("Big button Pressed");
				button->setTexture(texture);
			}
		}
	}
}