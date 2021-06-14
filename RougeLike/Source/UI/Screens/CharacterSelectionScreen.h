#pragma once

#include "Screen.h"
#include "UI/Elements/UISwitch.h"
#include "Actors/Player/Character.h"


class CharacterSelectionScreen : public Screen
{
public:
	CharacterSelectionScreen() : mTutorialFileState(UISwitch::Off), mCharacterIndex(0) { }

	void init() override;
	void handleInput(const InputManager* input) override;
	void slowUpdate() override;
	void exit() override { };

	const Character& selectedCharacter() const;
	const Character& nextCharacter() const;

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
	std::vector<Character> mCharacters;
};