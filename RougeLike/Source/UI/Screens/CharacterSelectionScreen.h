#pragma once

#include "Screen.h"
#include "UI/Elements/UISwitch.h"


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
	void readCharacters();
	void updateCharacter();


private:
	BasicString mSelectedCharacter;
	BasicString mSelectedWeapon;

	UISwitch::State mTutorialFileState;

	int mCharacterIndex;
	std::vector<BasicString> mCharacters;
};