#include "pch.h"
#include "PauseScreen.h"

#include "Game/GameData.h"
#include "UI/UIManager.h"
#include  "Graphics/TextureManager.h"

#include "UI/Elements/UIButton.h"


PauseScreen::PauseScreen(GameData* gameData) :
	Screen(gameData), 
	mResumeGame(false), mQuitGame(false), mRestartGame(false) { }




void PauseScreen::update()
{
	updateBoxTexture("ResumeButton", "ResumePanel");
	updateBoxTexture("SettingsButton", "SettingsPanel");
	updateBoxTexture("RestartButton", "RestartPanel");
	updateBoxTexture("QuitButton", "QuitPanel");

	UIButton* resumeButton = mGameData->uiManager->findButton("ResumeButton");
	if (resumeButton)
	{
		if (resumeButton->isReleased())
		{
			mResumeGame = true;
		}
	}

	UIButton* settingsButton = mGameData->uiManager->findButton("SettingsButton");
	if (settingsButton)
	{
		if (settingsButton->isReleased())
		{
			mOpenSettings = true;
		}
	}

	UIButton* restartButton = mGameData->uiManager->findButton("RestartButton");
	if (restartButton)
	{
		if (restartButton->isReleased())
		{
			mRestartGame = true;
		}
	}

	UIButton* quitButton = mGameData->uiManager->findButton("QuitButton");
	if (quitButton)
	{
		if (quitButton->isReleased())
		{
			mQuitGame = true;
		}
	}


}


// Change the box texture when the button is pressed
void PauseScreen::updateBoxTexture(std::string buttonId, std::string boxId)
{
	UIButton* button = mGameData->uiManager->findButton(buttonId);

	if (button)
	{
		UIBox* boxComponent = static_cast<UIBox*>(mGameData->uiManager->findElement(boxId));

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


void PauseScreen::exit()
{
	mResumeGame = false;
	mQuitGame = false;
	mRestartGame = false;
}