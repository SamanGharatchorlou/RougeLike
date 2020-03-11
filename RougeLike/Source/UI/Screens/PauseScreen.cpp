#include "pch.h"
#include "PauseScreen.h"
#include "Game/GameData.h"


PauseScreen::PauseScreen(GameData* gameData, std::vector<UILayer*> layers) :
	Screen(gameData, layers), 
	mResumeGame(false), mQuitGame(false), mRestartGame(false) { }


void PauseScreen::update()
{
	while (!mActions.empty())
	{
		switch (mActions.front())
		{
		case UIButton::ResumeGame_1:
			mResumeGame = true;
			break;
		case UIButton::QuitGame_2:
			mQuitGame = true;
			break;
		case UIButton::RestartGame_3:
			mRestartGame = true;
		default:
			break;
		}

		mActions.pop();
	}
}

void PauseScreen::exit()
{
	mResumeGame = false;
	mQuitGame = false;
	mRestartGame = false;
}