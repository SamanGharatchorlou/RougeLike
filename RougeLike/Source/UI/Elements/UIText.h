#pragma once

#include "UIElement.h"
#include "Graphics/Text.h"


class UIText : public UIElement
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
	UIText(Data& data);

	void setText(std::string text) { mText.setText(text); }

	void render() override;

	virtual bool isButton() { return false; }
	virtual Type type() { return Type::Text; }

private:
	Text mText;
};