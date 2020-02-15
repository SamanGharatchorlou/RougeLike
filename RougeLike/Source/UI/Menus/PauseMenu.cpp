#include "pch.h"
#include "PauseMenu.h"

#include "Game/GameData.h"


PauseMenu::PauseMenu(GameData* gameData) :
	Menu(gameData), mResumeGame(false), mQuitGame(false) { }


void PauseMenu::update()
{
	while (!mActions.empty())
	{
		switch (mActions.front())
		{
		case UIButton::RESUME_GAME_1:
			mResumeGame = true;
			break;
		case UIButton::QUIT_GAME_2:
			mQuitGame = true;
			break;
		default:
			break;
		}

		mActions.pop();
	}
}

void PauseMenu::exit()
{
	mResumeGame = false;
	mQuitGame = false;
}