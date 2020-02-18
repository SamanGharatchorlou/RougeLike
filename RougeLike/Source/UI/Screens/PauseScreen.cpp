#include "pch.h"
#include "PauseScreen.h"

// TODO: can remove?
#include "Game/GameData.h"


PauseScreen::PauseScreen(GameData* gameData, std::vector<UILayer*> layers) :
	Screen(gameData, layers), 
	mResumeGame(false), mQuitGame(false) { }


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
}