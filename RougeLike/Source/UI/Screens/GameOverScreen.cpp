#include "pch.h"
#include "GameOverScreen.h"

#include "UI/UIManager.h"
#include "UI/ScreenController.h"

#include "UI/Screens/Screen.h"
#include "UI/Elements/UIElement.h"
#include "UI/Elements/UITextBox.h"

void GameOverScreen::init()
{
	linkButton(ScreenItem::Continue, "ContinueButton");

	Screen* screen = mController->ui()->screen(ScreenType::Game);

	setTextBox("Kills", mKills);
	setTextBox("Score", mScore);
	setTextBox("Map Level", mMapLevel);
}


void GameOverScreen::slowUpdate()
{	
	if (released(ScreenItem::Continue))
	{
		mController->restartGame();
	}
}


void GameOverScreen::setTextBox(const BasicString& textBoxName, int value)
{
	UITextBox* textBox = findTextBox(textBoxName);
	textBox->setText(textBoxName + ": " + BasicString((float)value));
	textBox->align();
}
