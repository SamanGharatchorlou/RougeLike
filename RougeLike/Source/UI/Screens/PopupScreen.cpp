#include "pch.h"
#include "PopupScreen.h"

#include "UI/ScreenController.h"
#include "UI/Elements/UITextBox.h"

#include "Input/InputManager.h"
#include "System/Files/TextFileReader.h"

#include "UI/Elements/UIButton.h"


void PopupScreen::init()
{
	linkButton(ScreenItem::Close, "OkButton");
}

void PopupScreen::build(const XMLNode& node)
{
	XMLNode title = node.child("Title");
	setTitle(title);

	XMLNode body = node.child("Body");
	setBody(body);
}


void PopupScreen::handleInput(const InputManager* input)
{
	if (input->isReleased(Button::Enter) || input->isReleased(Button::Esc))
	{
		exitPopup();
	}
}


void PopupScreen::slowUpdate()
{
	if (released(ScreenItem::Close))
	{
		exitPopup();
	}
}


void PopupScreen::setMainText(const BasicString& textFile)
{
	const BasicString filePath = FileManager::Get()->findFile(FileManager::Config_Menus, textFile);
	TextFileReader reader(filePath.c_str());

	BasicString text("");
	reader.readText(text);

	UIElement* element = find("PopupText");
	UITextBox* textBox = static_cast<UITextBox*>(element);
	textBox->setText(text);
	textBox->autoSizeWrapFont();
}



void PopupScreen::setTitle(const XMLNode& titleNode)
{
	UIElement* element = find("Title");
	UITextBox* textBox = static_cast<UITextBox*>(element);
	textBox->setText(titleNode.value());
	textBox->align();
}

void PopupScreen::setBody(const XMLNode& bodyNode)
{
	UIElement* element = find("Body");
	UITextBox* textBox = static_cast<UITextBox*>(element);
	textBox->setText(bodyNode.value());
	textBox->autoSizeWrapFont();
}


void PopupScreen::exitPopup()
{
	mController->popScreen();
	mController->popSystemState();

	mButtons[ScreenItem::Close]->setActive(false);
}