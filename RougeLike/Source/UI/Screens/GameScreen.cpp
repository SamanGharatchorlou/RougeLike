#include "pch.h"
#include "GameScreen.h"


GameScreen::GameScreen(GameData* gameData) : Screen(gameData)
{ 

}

void GameScreen::render()
{
	Screen::render();

	for (int i = 0; i < mAbilityHotKeys.size(); i++)
	{
		mAbilityHotKeys[i].render();
	}
}