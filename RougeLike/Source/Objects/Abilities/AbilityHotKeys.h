#pragma once

#include "Input/Button.h"
#include "UI/Elements/UIBox.h"

struct GameData;
class AbilityManager;
class Ability;


class AbilityHotKeys
{
public:
	AbilityHotKeys(GameData* gameData, AbilityManager* abilities) : mGameData(gameData), mAbilities(abilities) { }

	void handleInput();

	void addHotKey(Ability* ability);


private:
	GameData* mGameData;
	AbilityManager* mAbilities;

	std::unordered_map<Button::Key, Ability*> hotKeyMap;
};