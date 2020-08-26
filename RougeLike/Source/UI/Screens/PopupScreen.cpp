#include "pch.h"
#include "PopupScreen.h"

#include "UI/ScreenController.h"
#include "UI/Elements/UITextBox.h"

#include "Input/InputManager.h"
#include "System/Files/TextFileReader.h"


void PopupScreen::init()
{
	linkButton(ScreenItem::Close, "OkButton");
}


void PopupScreen::handleInput(const InputManager* input)
{
	if (input->isPressed(Button::Enter) || input->isPressed(Button::Esc))
	{
		exitPopup();
	}
}


void PopupScreen::slowUpdate()
{
	if (selected(ScreenItem::Close))
	{
		exitPopup();
	}
}


void PopupScreen::setMainText(const BasicString& text)
{
	BasicString myText("");
	TextFileReader reader(FileManager::Get()->findFile(FileManager::Config_Menus, "LevelUp").c_str());
	reader.readText(myText);

	UIElement* element = find("PopupText");
	UITextBox* textBox = static_cast<UITextBox*>(element);
	textBox->setText(myText);
	textBox->autoSizeWrapFont();
}



void PopupScreen::exitPopup()
{
	mController->popScreen();
	mController->popSystemState();
}