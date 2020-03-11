#pragma once

#include "Screen.h"

struct GameData;

class CharacterSelectionScreen : public Screen
{
public:
	CharacterSelectionScreen(GameData* gameData, std::vector<UILayer*> layers);

	void enter() override { }
	void exit() override { };
	void update() override;

	const std::string& selectCharacter() const { return mSelectedCharacter; }

	bool enterGame() const { return mEnterGame; }

	Type type() override { return Type::CharacterSelection; }

private:
	std::string mSelectedCharacter;

	bool mEnterGame;
};