#pragma once

#include "Screen.h"


class CharacterSelectionScreen : public Screen
{
public:
	CharacterSelectionScreen(ScreenController* controller) : Screen(controller) { }
	void enter() override;
	void update() override { }
	void exit() override { };

	const BasicString& selectedCharacter() const { return mSelectedCharacter; }
	const BasicString& selectedWeapon() const { return mSelectedWeapon; }

	ScreenType type() override { return ScreenType::CharacterSelection; }


private:
	BasicString mSelectedCharacter;
	BasicString mSelectedWeapon;
};