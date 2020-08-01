#include "pch.h"
#include "AbilityHotKeys.h"

#include "Input/InputManager.h"

#include "Graphics/TextureManager.h"
#include "Graphics/Texture.h"

#include "UI/UIManager.h"
#include "UI/Screens/GameScreen.h"
#include "UI/Elements/UIBox.h"
#include "UI/Elements/UITextBox.h"


AbilityHotKeys::AbilityHotKeys(TextureManager* textures, Screen* screen) :  mTextures(textures), mGameScreen(static_cast<GameScreen*>(screen)) { }


Button::State AbilityHotKeys::state(AbilityType ability, const InputManager* input)
{
	return input->state(mHotKeyMap[ability]);
}


void AbilityHotKeys::addHotKey(AbilityType ability)
{
	int hotKeyCount = mHotKeyMap.size();
	int keyNumber = hotKeyCount + static_cast<int>('1');
	Button::Key buttonKey = static_cast<Button::Key>(keyNumber);
	mHotKeyMap[ability] = buttonKey;

	// Add ability icon
	UIBox* box = createIcon(ability, hotKeyCount);
	UITextBox* text = createIconText(box, hotKeyCount);

	mGameScreen->addElement(box);
	mGameScreen->addElement(text);
}


UIBox* AbilityHotKeys::createIcon(AbilityType item, int keyNumber)
{
	std::string id;
	item >> id;

	Texture* icon = mTextures->getTexture(id + "Icon", FileManager::Image_UI);
	VectorF position(75.0f * (1 + keyNumber), 650);
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
	textData.id = icon->id() + "IconText";
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