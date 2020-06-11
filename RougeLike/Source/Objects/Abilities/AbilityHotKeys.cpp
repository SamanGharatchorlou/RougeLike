#include "pch.h"
#include "AbilityHotKeys.h"

#include "AbilityManager.h"
#include "Game/GameData.h"
#include "Input/InputManager.h"
#include "Graphics/TextureManager.h"
#include "Graphics/Texture.h"

#include "UI/UIManager.h"
#include "UI/Screens/GameScreen.h"


#include "UI/Elements/UIBox.h"
#include "UI/Elements/UITextBox.h"

#include "Events/Events.h"


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

			mAbilities->sendSetTextColourEvent(iter->second, Colour::Green);
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


	// Add ability icon
	UIBox* box = createIcon(ability, count);
	UITextBox* text = createIconText(box, count);

	Screen* activeScreen = mGameData->uiManager->getActiveScreen();
	ASSERT(Warning, activeScreen->type() == Screen::Game, "Doesnt make sense not to be on the Game Screen, current screen: %d\n", activeScreen->type());
	GameScreen* gameScreen = static_cast<GameScreen*>(activeScreen);
	
	gameScreen->addElement(box);
	gameScreen->addElement(text);
}


// --- Private Functions --- //
UIBox* AbilityHotKeys::createIcon(Ability* ability, int count)
{
	std::string id = ability->name() + "Icon";
	Texture* icon = mGameData->textureManager->getTexture(id, FileManager::Image_UI);

	VectorF position(75.0f * (1 + count), 650);
	VectorF size = realiseSize(icon->originalDimentions, 50);
	RectF rect(position, size);

	UIBox::Data data;
	data.id = id;
	data.rect = rect;
	data.texture = icon;

	return new UIBox(data);
}


UITextBox* AbilityHotKeys::createIconText(UIBox* icon, int count)
{
	UITextBox::Data textData;
	textData.id = icon->id() + "Text";
	textData.aligment = "Center";
	textData.font = "";
	textData.ptSize = 48;
	textData.colour = SDL_Color{ 255, 255, 255 };
	textData.texture = nullptr;
	textData.rect = RectF();

	int number = count + 1;
	char buffer[5];
	_itoa_s(number, buffer, 10);
	textData.text = std::string(buffer);

	UITextBox* text = new UITextBox(textData);
	text->autoSizeRectToText();

	RectF rect = text->rect();
	rect.SetTopCenter(icon->rect().BotCenter());
	text->setRect(rect);

	return text;
}