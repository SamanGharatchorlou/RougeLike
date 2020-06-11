#pragma once

#include "Input/Button.h"
#include "Events/LocalDispatcher.h"

struct GameData;
class AbilityManager;
class Ability;
class UIBox;
class UITextBox;

class AbilityHotKeys
{
public:
	AbilityHotKeys(GameData* gameData, AbilityManager* abilities) : mGameData(gameData), mAbilities(abilities) { }

	void handleInput();

	void addHotKey(Ability* ability);

	LocalDispatcher events;


private:
	UIBox* createIcon(Ability* ability, int count);
	UITextBox* createIconText(UIBox* icon, int count);


private:
	GameData* mGameData;
	AbilityManager* mAbilities;

	std::unordered_map<Button::Key, Ability*> hotKeyMap;
};