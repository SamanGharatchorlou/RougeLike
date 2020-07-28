#pragma once
/*
#include "Input/Button.h"

// TEMP
#include "AbilityTypes/Ability.h"

class Screen;
class GameScreen;
class InputManager;
class Ability;
class UIBox;
class UITextBox;
class TextureManager;


template<typename T>
class HotKeys
{
public:
	HotKeys(Screen* screen) : mGameScreen(screen) { }

	Button::State state(T item, const InputManager* input);

	void addHotKey(T item);


protected:
	virtual UIBox* createIcon(T item, int keyNumber, TextureManager* textureManager) = 0;
	virtual UITextBox* createIconText(UIBox* icon, int keyNumber) = 0;


private:
	GameScreen* mGameScreen;
	std::unordered_map<T, Button::Key> mHotKeyMap;
};


template<typename T>
Button::State HotKeys<T>::state(T item, const InputManager* input)
{
	return input->state(mHotKeyMap[item]);
}


template<typename T>
void HotKeys<T>::addHotKey(T item)
{
	int hotKeyCount = mHotKeyMap.size();
	int keyNumber = hotKeyCount + static_cast<int>('0');
	Button::Key buttonKey = static_cast<Button::Key>(keyNumber);
	hotKeyMap[buttonKey] = item;

	// Add ability icon
	UIBox* box = createIcon(ability, count);
	UITextBox* text = createIconText(box, count);

	mGameScreen->addElement(box);
	mGameScreen->addElement(text);
}


class AbilityHotKeys : public HotKeys<AbilityType>
{
public:
	AbilityHotKeys(Screen* screen) : HotKeys(screen) { }

private:
	UIBox* createIcon(AbilityType item, int keyNumber, TextureManager* textureManager) override;
	UITextBox* createIconText(UIBox* icon, int keyNumber) override;

};

*/