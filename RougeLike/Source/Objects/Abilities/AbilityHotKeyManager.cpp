#include "pch.h"
#include "AbilityHotKeyManager.h"

#include "Input/InputManager.h"

#include "AbilityClasses/Ability.h"

#include "UI/Screens/GameScreen.h"
#include "UI/ScreenBuilder.h"
#include "UI/ScreenLayers.h"


// IMPROVEMENT
// Add a cooldown machanic where a blue box or something reduces in size
// showing how much cool down is left 
AbilityHotKeyManager::AbilityHotKeyManager() : mGameScreen(nullptr) { }


void AbilityHotKeyManager::addHotKey(Ability* ability)
{
	int hotKeyNumber = mHotKeys.size() + 1;
	HotKey hotKey(ability, hotKeyNumber);

	mHotKeys.push_back(hotKey);
	addToScreen(hotKey);
}


void AbilityHotKeyManager::updateStates()
{
	for (int i = 0; i < mHotKeys.size(); i++)
	{
		mHotKeys[i].updateButtonStateTextures(mStateButtons);
	}
}


HotKey* AbilityHotKeyManager::hotKey(Ability* ability)
{
	for (int i = 0; i < mHotKeys.size(); i++)
	{
		if (mHotKeys[i].mAbility == ability)
			return &mHotKeys[i];
	}

	DebugPrint(Warning, "No hot key found for ability %s\n", ability->name().c_str());
	return nullptr;
}


void AbilityHotKeyManager::addToScreen(HotKey& hotKey)
{
	ScreenLayer* layer = mGameScreen->layer("HotKeys");
	layer->addElements(hotKey.getComponents());
}



void AbilityHotKeyManager::init(Screen* screen)
{
	mGameScreen = static_cast<GameScreen*>(screen);

	const ScreenBuilder builder;
	TexturePacket buttonTextures = builder.getButtonTextures("TrafficLightSquareButton");
	mStateButtons[AbilityState::Idle] = buttonTextures.defaultTexture; // Green
	mStateButtons[AbilityState::Selected] = buttonTextures.selected; // Blue
	mStateButtons[AbilityState::Cooldown] = buttonTextures.hovering; // Red
}


void AbilityHotKeyManager::clear()
{
	mGameScreen = nullptr;
	mHotKeys.clear();
}

Button::State AbilityHotKeyManager::state(Ability* ability, const InputManager* input)
{
	return input->state(hotKey(ability)->mKey);
}


void AbilityHotKeyManager::setCursorTexture(Texture* texture)
{
	mGameScreen->setCursorTexture(texture);
}