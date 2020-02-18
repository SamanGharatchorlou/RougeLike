#pragma once

#include "UIElement.h"
#include "UIBox.h"
#include "Graphics/Text.h"


class UIBasicText : public UIElement
{
public:
	struct Data : public UIElement::Data
	{
		std::string text;
		std::string font;
		int ptSize;
		SDL_Color colour;
	};

public:
	UIBasicText(Data& data);

	void setText(std::string text) { mText.setText(text); }

	void render() override;

	virtual bool isButton() { return false; }
	virtual Type type() { return Type::BasicText; }

private:
	Text mText;
};



class UITextBox : public UIBox
{
public:
	struct Data : public UIBox::Data
	{
		std::string text;
		std::string font;
		int ptSize;
		SDL_Color colour;
	};

public:
	UITextBox(Data& data);

	void setText(std::string text) { mText.setText(text); }

	void render() override;

	virtual bool isButton() { return false; }
	virtual Type type() { return Type::TextBox; }


private:
	Text mText;
};