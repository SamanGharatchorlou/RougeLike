#include "pch.h"
#include "GameScreen.h"

#include "UI/UIManager.h"
#include "Input/InputManager.h"
#include "UI/ScreenLayers.h"
#include "Audio/AudioManager.h"

#include "UI/ScreenController.h"
#include "Game/GameController.h"

#include "UI/Elements/UIButton.h"
#include "UI/Elements/UISlider.h"

#include "Graphics/TextureManager.h"
#include "Graphics/Texture.h"

#if SET_GAME_SCALE
#include "Graphics/Renderer.h"
#endif


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

	mController->openPopup("Introduction");

#if SET_GAME_SCALE
	Renderer::Get()->setScale(game_scale);
#endif
}

void GameScreen::handleInput(const InputManager* input)
{
	if (input->isReleased(Button::Pause) || input->isReleased(Button::Esc))
	{
		mController->addScreen(ScreenType::Pause);
		mController->addSystemState(SystemStates::PauseState);
	}
}


// Override this so we dont have any cursor or button clicking sounds
void GameScreen::updateInputs(const InputManager* input)
{
	updateButtons(input);
	updateSliders(input);
	updateSwitches(input);
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


void GameScreen::setCursorTexture(Texture* texture)
{
	mController->ui()->setCursorTexture(texture);
}


void GameScreen::resume()
{
	mController->setPersistingScreen(nullptr);
	mController->ui()->setCursorTexture(TextureManager::Get()->getTexture("GameCursor", FileManager::Image_UI));
}


void GameScreen::pause()
{
	exit();
}

void GameScreen::exit()
{
	mController->setPersistingScreen(this);
	mController->ui()->setCursorTexture(TextureManager::Get()->getTexture("UICursor", FileManager::Image_UI));
}