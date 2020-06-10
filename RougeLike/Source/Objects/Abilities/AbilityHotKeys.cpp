#include "pch.h"
#include "AbilityHotKeys.h"

#include "AbilityManager.h"
#include "Game/GameData.h"
#include "Input/InputManager.h"
#include "Graphics/TextureManager.h"
#include "Graphics/Texture.h"

#include "UI/UIManager.h"
#include "UI/Screens/GameScreen.h"


void AbilityHotKeys::handleInput()
{
	for (std::unordered_map<Button::Key, Ability*>::iterator iter = hotKeyMap.begin();
		iter != hotKeyMap.end(); iter++)
	{
		Button::Key key = iter->first;

		
		if (mGameData->inputManager->isPressed(key))
		{
			mAbilities->exitSelection();
			mAbilities->setState(iter->second, Ability::Selected);
		}
		//else if (mInput->isReleased(key))
		//{
		//	mAbilities->exitSelection(iter->second);
		//}
	}
}


void AbilityHotKeys::addHotKey(Ability* ability)
{
	int count = 0;

	for (std::unordered_map<Button::Key, Ability*>::iterator iter = hotKeyMap.begin();
		iter != hotKeyMap.end(); iter++)
	{
		count++;
	}

	int key = static_cast<int>('0') + 1 + count;
	Button::Key buttonKey = static_cast<Button::Key>(key);
	hotKeyMap[buttonKey] = ability;


	// add to ui
	Texture* icon = mGameData->textureManager->getTexture(ability->name() + "Icon", FileManager::Image_UI);
	
	VectorF position(75.0f * (1 + count), 650);
	VectorF size = realiseSize(icon->originalDimentions, 50);
	RectF rect(position, size);

	UIBox::Data data;
	data.id = ability->name();
	data.rect = rect;
	data.texture = icon;

	UIBox box(data);

	Screen* activeScreen = mGameData->uiManager->getActiveScreen();
	ASSERT(Warning, activeScreen->type() == Screen::Game, "Doesnt make sense not to be on the Game Screen, current screen: %d\n", activeScreen->type());
	GameScreen* gameScreen = static_cast<GameScreen*>(activeScreen);
	
	gameScreen->AddBox(box);
}