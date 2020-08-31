#include "pch.h"
#include "CharacterSelectionScreen.h"

#include "Input/InputManager.h"
#include  "Graphics/TextureManager.h"

#include "Game/GameController.h"
#include "UI/ScreenController.h"
#include "Game/GameSetup.h"

#include "UI/Elements/UIBox.h"
#include "UI/Elements/UIButton.h"
#include "UI/Elements/UISwitch.h"


void CharacterSelectionScreen::init()
{
	const TextureManager* textures = TextureManager::Get();
	UIBox* character = static_cast<UIBox*>(find("Character"));
	mSelectedCharacter = "Soldier";

	Texture* texture = textures->getTexture(mSelectedCharacter + "Icon", FileManager::Image_UI);
	character->setTexture(texture);

	mSelectedWeapon = "Sword";

	linkButton(ScreenItem::Play, "PlayButton");
	linkSwitch(ScreenItem::Tutorial, "TutorialSwitch");

	XMLParser parser(FileManager::Get()->findFile(FileManager::Configs, "GameSettings"));
	BasicString switchState = parser.rootChild("Tutorial").value();

	mTutorialFileState = switchState == "ON" ? UISwitch::On : UISwitch::Off;
	mSwitches[ScreenItem::Tutorial]->setState(mTutorialFileState);
}


void CharacterSelectionScreen::handleInput(const InputManager* input)
{
	if (input->isPressed(Button::Esc) || input->isPressed(Button::Quit))
	{
		mController->quitGame();
	}
#if !UI_EDITOR
	else if (input->isPressed(Button::Enter))
	{
		mController->replaceScreen(ScreenType::Game);
		mController->replaceSystemState(SystemStates::GameState);
	}
#endif
}

void CharacterSelectionScreen::slowUpdate()
{
	if (selected(ScreenItem::Play))
	{
		mController->replaceScreen(ScreenType::Game);
		mController->replaceSystemState(SystemStates::GameState);

		UISwitch::State tutorialState = mSwitches[ScreenItem::Tutorial]->state();
		if (mTutorialFileState != tutorialState)
		{
			const BasicString mode = (tutorialState == UISwitch::On) ? "ON" : "OFF";
			GameSetup::setTutorial(mode);

		}
		mController->enablePopups((bool)tutorialState);
	}
}