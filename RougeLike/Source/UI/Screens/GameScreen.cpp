#include "pch.h"
#include "GameScreen.h"

#include "UI/UIManager.h"
#include "Input/InputManager.h"
#include "UI/ScreenLayers.h"

#include "UI/ScreenController.h"
#include "Game/GameController.h"

#include "UI/Elements/UIButton.h"
#include "UI/Elements/UISlider.h"

#include "Graphics/TextureManager.h"
#include "Graphics/Texture.h"



GameScreen::GameScreen()
{ 
	ScreenLayer layer("HotKeys");
	add(layer);
}



void GameScreen::init()
{
	linkSlider(ScreenItem::Health, "HealthSlider");
	linkSlider(ScreenItem::Exp, "LevelSlider");

	slider(ScreenItem::Health)->disableInput();

	slider(ScreenItem::Exp)->disableInput();
	slider(ScreenItem::Exp)->setSliderValue(0.0f);

	mController->ui()->setCursorTexture(TextureManager::Get()->getTexture("GameCursor", FileManager::Image_UI));


#if SET_GAME_SCALE
	Renderer::Get()->setScale(game_scale);
#endif
}

void GameScreen::handleInput(const InputManager* input)
{
	if (input->isPressed(Button::Esc))
	{
		mController->quitGame();
	}

	if (input->isReleased(Button::Pause))
	{
		mController->addScreen(ScreenType::Pause);
		mController->addSystemState(SystemStates::PauseState);
	}
}


void GameScreen::slowUpdate()
{
	if (slider(ScreenItem::Health)->getValue() == 0.0f)
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


void GameScreen::resume()
{
	mController->setPersistingScren(nullptr);
}


void GameScreen::pause()
{
	mController->setPersistingScren(this);
}