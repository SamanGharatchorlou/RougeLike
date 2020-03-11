#include "pch.h"
#include "CharacterSelectionScreen.h"



CharacterSelectionScreen::CharacterSelectionScreen(GameData* gameData, std::vector<UILayer*> layers) :
	Screen(gameData, layers), mEnterGame(false)
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
		}

		mActions.pop();
	}
}