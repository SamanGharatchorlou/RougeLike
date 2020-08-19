#pragma once

#include "Screen.h"


class CharacterSelectionScreen : public Screen
{
public:
	void enter() override;
	void update() override { }
	void exit() override { };

	const BasicString& selectedCharacter() const { return mSelectedCharacter; }
	const BasicString& selectedWeapon() const { return mSelectedWeapon; }

	Type type() override { return Type::CharacterSelection; }


private:
	BasicString mSelectedCharacter;
	BasicString mSelectedWeapon;
};