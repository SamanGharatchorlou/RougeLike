#include "pch.h"
#include "CharacterSelectionScreen.h"

#include "Input/InputManager.h"
#include  "Graphics/TextureManager.h"
#include "Audio/AudioManager.h"

#include "Game/GameController.h"
#include "UI/ScreenController.h"
#include "Game/GameSetup.h"

#include "UI/Elements/UIBox.h"
#include "UI/Elements/UIButton.h"
#include "UI/Elements/UITextBox.h"
#include "UI/Elements/UISwitch.h"

#include "Weapons/WeaponData.h"
#include "Actors/Player/Character.h"


void CharacterSelectionScreen::init()
{
	readCharacters();
	updateCharacter();

	linkButton(ScreenItem::Play, "PlayButton");
	linkButton(ScreenItem::Quit, "ExitButton");
	linkButton(ScreenItem::LeftArrow, "LeftButton");
	linkButton(ScreenItem::RightArrow, "RightButton");
	linkSwitch(ScreenItem::Tutorial, "TutorialSwitch");

	XMLParser settingsParser(FileManager::Get()->findFile(FileManager::Configs, "GameSettings"));
	BasicString switchState = settingsParser.rootChild("Tutorial").value();

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
		updateCharacter();
		enterGame();
	}
#endif
}



void CharacterSelectionScreen::slowUpdate()
{
	if (released(ScreenItem::Play))
	{
		updateCharacter();
		enterGame();
	}

	if (released(ScreenItem::Quit))
	{
		mController->quitGame();
	}

	if (released(ScreenItem::LeftArrow))
	{
		mCharacterIndex = mCharacterIndex - 1 < 0 ? mCharacters.size() - 1 : mCharacterIndex - 1;
		updateCharacter();
	}

	if (released(ScreenItem::RightArrow))
	{
		mCharacterIndex = mCharacterIndex + 1 >= mCharacters.size() ? 0 : mCharacterIndex + 1;
		updateCharacter();
	}
}


// -- Private Funtions -- //

void CharacterSelectionScreen::readCharacters()
{
	XMLParser characterParser(FileManager::Get()->findFile(FileManager::Configs, "Characters"));
	XMLNode characterNode = characterParser.rootNode().child("PlayerCharacter");
	while (characterNode)
	{
		Character character;
		character.mName = characterNode.child("Name").value();
		character.mWeaponName = characterNode.child("WeaponName").value();

		BasicString weaponType = characterNode.child("WeaponType").value();

		if (weaponType == "Melee")
			character.mWeaponType = WeaponType::Melee;
		else if (weaponType == "Magic")
			character.mWeaponType = WeaponType::Magic;
		else
			DebugPrint(Warning, "No valid weapon type was found for type %s. Check the %s file\n", 
				weaponType.c_str(), FileManager::Get()->findFile(FileManager::Configs, "Characters").c_str());

		mCharacters.push_back(character);
		characterNode = characterNode.next();
	}
}

void CharacterSelectionScreen::updateCharacter()
{
	const TextureManager* textures = TextureManager::Get();
	Texture* texture = TextureManager::Get()->getTexture(selectedCharacter().mName + "Icon", FileManager::Image_UI);

	UIBox* character = static_cast<UIBox*>(find("Character"));
	character->setTexture(texture);

	UITextBox* textBox = static_cast<UITextBox*>(find("CharacterName"));
	BasicString textString = selectedCharacter().mName + " - " + selectedCharacter().mWeaponName;
	textBox->setText(textString);
	textBox->align();
}


void CharacterSelectionScreen::enterGame()
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
	AudioManager::Get()->pushEvent(AudioEvent(AudioEvent::Play, "StartGame", mController));
}



const Character& CharacterSelectionScreen::selectedCharacter() const 
{ 
	return mCharacters.at(mCharacterIndex);
}