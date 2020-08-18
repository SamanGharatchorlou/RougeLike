#pragma once

#include "AbilityClasses/AbilityStates.h"
#include "Input/Button.h"

class InputManager;
class Screen;
class GameScreen;
class UIBox;
class UITextBox;


class AbilityHotKeys
{
public:
	AbilityHotKeys();

	void init(Screen* screen);
	void clear();

	Button::State state(AbilityType ability, const InputManager* input);
	void addHotKey(AbilityType ability);


private:
	UIBox* createIcon(AbilityType item, int keyNumber);
	UITextBox* createIconText(UIBox* icon, int keyNumber);


private:
	GameScreen* mGameScreen;
	std::unordered_map<AbilityType, Button::Key> mHotKeyMap;
};

