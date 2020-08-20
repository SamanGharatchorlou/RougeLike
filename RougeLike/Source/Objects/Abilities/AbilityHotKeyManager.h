#pragma once

#include "Input/Button.h"
#include "AbilityHotKey.h"

class InputManager;
class Screen;
class GameScreen;
class Texture;
class Ability;
enum class AbilityState;


class AbilityHotKeyManager
{
public:
	AbilityHotKeyManager();

	void init(Screen* screen);
	void clear();

	Button::State state(Ability* ability, const InputManager* input);
	void addHotKey(Ability* ability);
	void addToScreen(HotKey& hotKey);

	void updateStates();
	HotKey* hotKey(Ability* ability);



private:
	GameScreen* mGameScreen;

	std::vector<HotKey> mHotKeys;
	std::unordered_map<AbilityState, Texture*> mStateButtons;
};

