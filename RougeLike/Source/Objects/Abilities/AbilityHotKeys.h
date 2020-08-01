#pragma once

#include "AbilityTypes/Ability.h"
#include "Input/Button.h"

class TextureManager;
class InputManager;
class Screen;
class GameScreen;
class UIBox;
class UITextBox;


class AbilityHotKeys
{
public:
	AbilityHotKeys(TextureManager* textures, Screen* screen);

	Button::State state(AbilityType ability, const InputManager* input);
	void addHotKey(AbilityType ability);


private:
	UIBox* createIcon(AbilityType item, int keyNumber);
	UITextBox* createIconText(UIBox* icon, int keyNumber);


private:
	TextureManager* mTextures;
	GameScreen* mGameScreen;
	std::unordered_map<AbilityType, Button::Key> mHotKeyMap;
};

