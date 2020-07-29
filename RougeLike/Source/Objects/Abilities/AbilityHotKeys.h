#pragma once

#include "Input/Button.h"
#include "UI/Screens/GameScreen.h"
#include "UI/Elements/UIBox.h"
#include "UI/Elements/UITextBox.h"


// TEMP
#include "AbilityTypes/Ability.h"
#include "Input/InputManager.h"

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
	HotKeys(GameScreen* gameScreen) : mGameScreen(gameScreen) { }

	Button::State state(T item, const InputManager* input);

	void addHotKey(T item, TextureManager* textureManager);


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
	Button but = input->getButton(mHotKeyMap[item]);

	return input->state(mHotKeyMap[item]);
}


template<typename T>
void HotKeys<T>::addHotKey(T item, TextureManager* textureManager)
{
	int hotKeyCount = mHotKeyMap.size();
	int keyNumber = hotKeyCount + static_cast<int>('1');
	Button::Key buttonKey = static_cast<Button::Key>(keyNumber);
	mHotKeyMap[item] = buttonKey;

	// Add ability icon
	UIBox* box = createIcon(item, hotKeyCount, textureManager);
	UITextBox* text = createIconText(box, hotKeyCount);

	mGameScreen->addElement(box);
	mGameScreen->addElement(text);
}


class AbilityHotKeys : public HotKeys<AbilityType>
{
public:
	AbilityHotKeys(Screen* screen) : HotKeys(static_cast<GameScreen*>(screen)) { }

private:
	UIBox* createIcon(AbilityType item, int keyNumber, TextureManager* textureManager) override;
	UITextBox* createIconText(UIBox* icon, int keyNumber) override;

};

