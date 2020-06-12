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

	const std::string& selectedCharacter() const { return mSelectedCharacter; }
	const std::string& selectedWeapon() const { return mSelectedWeapon; }

	bool enterGame() const { return mEnterGame; }

	Type type() override { return Type::CharacterSelection; }

private:
	std::string mSelectedCharacter;
	std::string mSelectedWeapon;

	bool mEnterGame;
};