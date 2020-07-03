#pragma once

#include "Input/Button.h"
#include "Events/LocalDispatcher.h"

class AbilityManager;
class InputManager;
class Ability;
class UIBox;
class UITextBox;

class AbilityHotKeys
{
public:
	AbilityHotKeys(AbilityManager* abilities) : mManager(abilities) { }

	void handleInput(InputManager* input);

	void addHotKey(Ability* ability);
	Button::Key hotKey(Ability* ability);

	LocalDispatcher events;


private:
	UIBox* createIcon(Ability* ability, int count);
	UITextBox* createIconText(UIBox* icon, int count);


private:
	AbilityManager* mManager;

	std::unordered_map<Button::Key, Ability*> hotKeyMap;
};