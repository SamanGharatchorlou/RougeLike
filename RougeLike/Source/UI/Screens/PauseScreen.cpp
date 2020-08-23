#include "pch.h"
#include "PauseScreen.h"

#include "Input/InputManager.h"
#include "UI/Elements/UIButton.h"


void PauseScreen::enter()
{
	mButtons.clear();

	linkButton(ScreenItem::Resume, "ResumeButton");
	linkButton(ScreenItem::Restart, "RestartButton");
	linkButton(ScreenItem::Settings, "SettingsButton");
	linkButton(ScreenItem::Quit, "QuitButton");
}


void PauseScreen::handleInput(const InputManager* input) 
{
	if (input->isPressed(Button::Pause))
	{
		mController->pushScreen(ScreenType::Popup);
	}
}


void PauseScreen::update()
{
	if (selected(ScreenItem::Settings))
	{
		mController->pushScreen(ScreenType::Settings);
	}

	if (selected(ScreenItem::Resume))
	{
		mController->popScreen();
	}
}