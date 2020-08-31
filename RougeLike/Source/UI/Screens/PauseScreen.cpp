#include "pch.h"
#include "PauseScreen.h"

#include "UI/ScreenController.h"
#include "Input/InputManager.h"
#include "UI/Elements/UIButton.h"


void PauseScreen::init()
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
		popScreenState();
	}
	else if (input->isPressed(Button::Esc) || input->isPressed(Button::Quit))
	{
		mController->quitGame();
	}
}


void PauseScreen::slowUpdate()
{
	if (selected(ScreenItem::Settings))
	{
		mController->addScreen(ScreenType::Settings);
	}

	if (selected(ScreenItem::Quit))
	{
		mController->quitGame();
	}
	else if (selected(ScreenItem::Restart))
	{
		mController->restartGame();
	}
	else if (selected(ScreenItem::Resume))
	{
		popScreenState();
	}
}


void PauseScreen::popScreenState()
{
	mController->popScreen();
	mController->popSystemState();
}