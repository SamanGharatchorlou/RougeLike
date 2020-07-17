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


void AbilityHotKeys::handleInput(const InputManager* input)
{
	// As per Adrian Grutters request, rename the iterator
	for (std::unordered_map<Button::Key, Ability*>::iterator retiTheGr8RulerOvDaLandz = hotKeyMap.begin();
		retiTheGr8RulerOvDaLandz != hotKeyMap.end(); retiTheGr8RulerOvDaLandz++)
	{
		Button::Key key = retiTheGr8RulerOvDaLandz->first;
		Ability* ability = retiTheGr8RulerOvDaLandz->second;

		if (input->isPressed(key))
		{
			mManager->setState(ability, Ability::Selected);
		}
		else if (input->isReleased(key))
		{
			mManager->setState(ability, Ability::Idle);
		}
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
	//UIBox* box = createIcon(ability, count);
	//UITextBox* text = createIconText(box, count);

	//Screen* activeScreen = mManager->mGameData->uiManager->getActiveScreen();
	//ASSERT(Warning, activeScreen->type() == Screen::Game, "Doesnt make sense not to be on the Game Screen, current screen: %d\n", activeScreen->type());
	//GameScreen* gameScreen = static_cast<GameScreen*>(activeScreen);
	//
	//gameScreen->addElement(box);
	//gameScreen->addElement(text);
}


Button::Key AbilityHotKeys::hotKey(Ability* ability)
{
	for (std::unordered_map<Button::Key, Ability*>::iterator iter = hotKeyMap.begin();
		iter != hotKeyMap.end(); iter++)
	{
		if (ability == iter->second)
			return iter->first;
	}

	DebugPrint(Log, "ability '%s' has no hotkey assigned\n", ability->name().c_str());
	return Button::Key::None;
}



// --- Private Functions --- //
UIBox* AbilityHotKeys::createIcon(Ability* ability, int count)
{
	std::string id = ability->name() + "Icon";
	Texture* icon = nullptr;// mManager->mGameData->textureManager->getTexture(id, FileManager::Image_UI);

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