#include "pch.h"
#include "GameScreen.h"

#include "Input/InputManager.h"
#include "UI/Elements/UIElement.h"
#include "UI/Elements/UIButton.h"
#include "UI/Elements/UISlider.h"

#include "Graphics/TextureManager.h"
#include "Graphics/Texture.h"

GameScreen::GameScreen()
{ 
	ScreenLayer layer("HotKeys");
	add(layer);
}



void GameScreen::enter()
{
	linkSlider(ScreenItem::Health, "HealthSlider");


	mSliders[ScreenItem::Health]->disableInput();
}

void GameScreen::handleInput(const InputManager* input)
{
	if (input->getButton(Button::E).isPressed())
	{
		std::unordered_map<AbilityType, Button::Key>::iterator iter = mHotkeyMap->begin();

		iter->second = Button::Two;
	}
}


void GameScreen::update(float dt)
{
	if (mSliders[ScreenItem::Health]->getValue() == 0.0f)
	{
		UIButton* icon = mSliders[ScreenItem::Health]->handle();
		Texture* texture = TextureManager::Get()->getTexture("DeadIcon", FileManager::Image_UI);
		icon->setTextures(texture, nullptr, nullptr);

		float maxDim = icon->rect().Height();
		VectorF newSize = realiseSize(texture->originalDimentions, maxDim);
		RectF newRect = icon->rect();
		newRect.SetSize(newSize);
		icon->setRect(newRect);
	}


}
