#include "pch.h"
#include "GameScreen.h"

#include "UI/Elements/UIElement.h"
#include "UI/Elements/UIButton.h"
#include "UI/Elements/UISlider.h"

#include "Graphics/TextureManager.h"

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


void GameScreen::update(float dt)
{
	if (mSliders[ScreenItem::Health]->getValue() == 0.0f)
	{
		UIButton* icon = mSliders[ScreenItem::Health]->handle();
		Texture* texture = TextureManager::Get()->getTexture("DeadIcon", FileManager::Image_UI);
		icon->setTextures(texture, nullptr, nullptr);
	}
}
