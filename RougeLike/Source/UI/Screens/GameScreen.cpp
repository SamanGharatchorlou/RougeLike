#include "pch.h"
#include "GameScreen.h"

#include "Graphics/TextureManager.h"
#include "Managers/ScoreManager.h"


GameScreen::GameScreen(GameData* gameData, std::vector<UILayer*> layers) :
	Screen(gameData, layers)
{ 
	//SDL_Color textColor = { 0, 225, 0 };
	//RectF rect(20, 20, 220, 70);

	//mScore.initText("lazy.ttf", 56, textColor, "Score: 0");

	//mButton.initText("lazy.ttf", 56, textColor, "Score: 0");
	//Texture* tex = mGameData->textureManager->getTexture("RedButton");
	//mButton.initButton(rect, tex, tex, UIButton::QuitGame_2);
}



void GameScreen::update()
{
	//if (mGameData->scoreManager->hasScoreChanged())
	//{
	//	std::string text = "Score: " + std::to_string(mGameData->scoreManager->score());
	//	mScore.setText(text);
	//}
}

//void GameScreen::render()
//{
//	Screen::render();
//
//	//mScore.render();
//	//mButton.render();
//}
