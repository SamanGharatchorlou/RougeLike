#include "pch.h"
#include "AbilityHotKeys.h"

#include "Input/InputManager.h"

#include "Graphics/TextureManager.h"
#include "Graphics/Texture.h"

#include "UI/UIManager.h"
#include "UI/Screens/GameScreen.h"
#include "UI/Elements/UIBox.h"
#include "UI/Elements/UITextBox.h"


AbilityHotKeys::AbilityHotKeys() : mGameScreen(nullptr) { }

void AbilityHotKeys::init(Screen* screen)
{
	mGameScreen = static_cast<GameScreen*>(screen);
}


void AbilityHotKeys::clear()
{
	mGameScreen = nullptr;
	mHotKeyMap.clear();
}

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

	ScreenLayer* layer = mGameScreen->layer("HotKeys");
	layer->elements.push_back(box);
	layer->elements.push_back(text);
}


UIBox* AbilityHotKeys::createIcon(AbilityType item, int keyNumber)
{
	BasicString id;
	item >> id;

	const TextureManager* textures = TextureManager::Get();
	Texture* icon = textures->getTexture(id + "Icon", FileManager::Image_UI);

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
	int number = count + 1;
	char buffer[5];
	_itoa_s(number, buffer, 10);

	UITextBox::Data textData;
	textData.text = BasicString(buffer);	
	textData.id = icon->id() + "IconText";
	textData.ptSize = 48;
	textData.colour = SDL_Color{ 255, 255, 255 };

	UITextBox* text = new UITextBox(textData);
	text->autoSizeRectToText();

	RectF rect = text->rect();
	rect.SetTopCenter(icon->rect().BotCenter());
	text->setRect(rect);

	return text;
}