#pragma once

#include "UIElement.h"
#include "UIBox.h"
#include "Graphics/Text.h"


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

	int fontSize() const { return mText.fontSize(); }
	void autoSizeFont();

	void render() override;

	virtual bool isButton() const { return false; }
	virtual bool hasText() const { return true; }
	virtual Type type() const { return Type::TextBox; }


private:
	Text mText;
};
