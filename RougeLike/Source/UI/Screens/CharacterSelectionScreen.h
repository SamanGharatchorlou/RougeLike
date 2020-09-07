#pragma once

#include "Screen.h"
#include "UI/Elements/UISwitch.h"

// Name + Weapon
using CharacterInfo = std::pair<BasicString, BasicString>;


class CharacterSelectionScreen : public Screen
{
public:
	CharacterSelectionScreen() : mTutorialFileState(UISwitch::Off), mCharacterIndex(0) { }

	void init() override;
	void handleInput(const InputManager* input) override;
	void slowUpdate() override;
	void exit() override { };

	const BasicString& selectedCharacter() const { return mSelectedCharacter; }
	const BasicString& selectedWeapon() const { return mSelectedWeapon; }

	ScreenType type() override { return ScreenType::CharacterSelection; }


private:
	void enterGame();
	void readCharacters();
	void updateCharacter();


private:
	BasicString mSelectedCharacter;
	BasicString mSelectedWeapon;

	UISwitch::State mTutorialFileState;

	int mCharacterIndex;
	std::vector<CharacterInfo> mCharacters;
};