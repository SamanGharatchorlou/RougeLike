#include "pch.h"
#include "PauseScreen.h"

#include "UI/Elements/UIButton.h"


void PauseScreen::enter()
{
	mButtons.clear();

	linkButton(ScreenItem::Resume, "ResumeButton");
	linkButton(ScreenItem::Restart, "RestartButton");
	linkButton(ScreenItem::Settings, "SettingsButton");
	linkButton(ScreenItem::Quit, "QuitButton");
}