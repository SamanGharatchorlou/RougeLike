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
	if (input->isReleased(Button::Pause))
	{
		popScreenState();
	}
	else if (input->isReleased(Button::Esc) || input->isReleased(Button::Quit))
	{
		mController->quitGame();
	}
}


void PauseScreen::slowUpdate()
{
	if (released(ScreenItem::Settings))
	{
		mController->addScreen(ScreenType::Settings);
	}

	if (released(ScreenItem::Quit))
	{
		mController->quitGame();
	}
	else if (released(ScreenItem::Restart))
	{
		mController->restartGame();
	}
	else if (released(ScreenItem::Resume))
	{
		popScreenState();
	}
}


void PauseScreen::popScreenState()
{
	mController->popScreen();
	mController->popSystemState();
}