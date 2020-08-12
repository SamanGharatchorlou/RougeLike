#include "pch.h"
#include "PauseScreen.h"

#include "Game/Data/GameData.h"
#include "UI/UIManager.h"
#include  "Graphics/TextureManager.h"

#include "UI/Elements/UIButton.h"


PauseScreen::PauseScreen(const TextureManager* textures) : Screen(textures) { }


void PauseScreen::enter()
{
	mButtons.clear();
	mStates.clear();
	setButtonsAndStates();
}


void PauseScreen::update(float dt)
{
	std::unordered_map<BasicString, UIButton*>::iterator iter;
	for (iter = mButtons.begin(); iter != mButtons.end(); iter++)
	{
		UIButton* button = iter->second;
		if (button->isReleased())
		{
			mStates[iter->first] = true;
		}
	}

	updateBoxTexture("ResumeButton", "ResumePanel");
	updateBoxTexture("SettingsButton", "SettingsPanel");
	updateBoxTexture("RestartButton", "RestartPanel");
	updateBoxTexture("QuitButton", "QuitPanel");
}


// TODO: replace input parameters with the button and elements rather than strings
// Change the box texture when the button is pressed
void PauseScreen::updateBoxTexture(BasicString buttonId, BasicString boxId)
{
	UIButton* button = findButton(buttonId);
	
	if (button)
	{
		UIBox* boxComponent = static_cast<UIBox*>(find(boxId));

		if (boxComponent)
		{
			Texture* texture;

			if (button->isHeld())
			{
				texture = mTextures->getTexture("Big button Pressed", FileManager::Image_UI);
			}
			else
			{
				texture = mTextures->getTexture("Big button Released", FileManager::Image_UI);
			}

			boxComponent->setTexture(texture);
		}
	}
}


void PauseScreen::exit()
{
	resetButtonStates();
}


void PauseScreen::setButtonsAndStates()
{
	addButtonAndState("ResumeButton");
	addButtonAndState("SettingsButton");
	addButtonAndState("RestartButton");
	addButtonAndState("QuitButton");
}


void PauseScreen::resetButtonStates()
{
	std::unordered_map<BasicString, bool>::iterator iter;
	for (iter = mStates.begin(); iter != mStates.end(); iter++)
	{
		iter->second = false;
	}
}


void PauseScreen::addButtonAndState(const BasicString& label)
{
	mButtons[label] = findButton(label);
	mStates[label] = false;
}