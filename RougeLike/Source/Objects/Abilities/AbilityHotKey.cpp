#include "pch.h"
#include "AbilityHotKey.h"

#include "Graphics/TextureManager.h"
#include "Graphics/Texture.h"

#include "UI/Elements/UIBox.h"
#include "UI/Elements/UITextBox.h"

#include "AbilityClasses/Ability.h"



HotKey::HotKey(Ability* ability, int count) : mAbility(ability)
{
	mID = count;

	int keyNumber = count + static_cast<int>('0');
	mKey = static_cast<Button::Key>(keyNumber);

	mIconBG = creatIconBG();
	mIcon = createIcon(mIconBG);
	mButton = createButton(mIconBG);
	mKeyText = createButtonText(mButton, BasicString((float)count));
}


Elements HotKey::getComponents()
{
	return Elements { mIconBG, mIcon, mButton, mKeyText };
}


UIBox* HotKey::creatIconBG() const
{
	VectorF position(75.0f * mID, 600.0f);
	VectorF size(75.0f, 75.0f);
	RectF rect(position, size);

	const TextureManager* textures = TextureManager::Get();
	const BasicString& id = mAbility->name() + "IconBG";
	Texture* texture = textures->getTexture("IconBG", FileManager::Image_UI);

	UIBox::Data data(id, rect, texture);
	return new UIBox(data);
}


UIBox* HotKey::createIcon(const UIBox* iconBg) const
{
	const TextureManager* textures = TextureManager::Get();

	const BasicString id = mAbility->name() + "Icon";
	Texture* texture = textures->getTexture(id, FileManager::Image_UI);

	VectorF size = realiseSize(texture->originalDimentions, 50.0f);
	RectF rect(VectorF(), size);
	rect.SetCenter(iconBg->rect().Center());

	UIBox::Data data(id, rect, texture);
	return new UIBox(data);
}


UIBox* HotKey::createButton(const UIBox* iconBG) const
{
	const BasicString id = mAbility->name() + "Button";
	RectF rect = iconBG->rect();
	rect.SetTopCenter(iconBG->rect().BotCenter());

	UIBox::Data data(id, rect, nullptr);
	return new UIBox(data);
}


UITextBox* HotKey::createButtonText(UIBox* button, const BasicString& text)
{
	UITextBox::Data textData;
	textData.text = text;
	textData.id = mAbility->name() + "Text";
	textData.ptSize = 48;
	textData.colour = SDL_Color{ 255, 255, 255 };

	UITextBox* textBox = new UITextBox(textData);
	textBox->autoSizeRectToText();

	RectF rect = textBox->rect();
	rect.SetCenter(button->rect().Center());
	textBox->setRect(rect);

	return textBox;
}


void HotKey::updateButtonState(std::unordered_map<AbilityState, Texture*>& buttonStates)
{
	Texture* texture = buttonStates[mAbility->state()];
	if (texture)
		mButton->setTexture(texture);
}

