#pragma once

#include "Screen.h"

struct GameData;

class CharacterSelectionScreen : public Screen
{
public:
	CharacterSelectionScreen(GameData* gameData);

	void enter() override;
	void exit() override { };
	void update(float dt) override;

	const BasicString& selectedCharacter() const { return mSelectedCharacter; }
	const BasicString& selectedWeapon() const { return mSelectedWeapon; }

	bool enterGame() const { return mEnterGame; }

	Type type() override { return Type::CharacterSelection; }

private:
	BasicString mSelectedCharacter;
	BasicString mSelectedWeapon;

	bool mEnterGame;
};