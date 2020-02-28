#pragma once

#include "UIElement.h"
#include "UIBox.h"
#include "Graphics/Text.h"


// TODO: merge this into UIText... just add a box texture
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