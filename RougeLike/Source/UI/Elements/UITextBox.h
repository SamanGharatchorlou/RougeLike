#pragma once

#include "UIBox.h"
#include "Text/Text.h"


class UITextBox : public UIBox
{
public:
	struct Data : public UIBox::Data
	{
		std::string aligment;
		std::string text;
		std::string font;
		int ptSize;
		SDL_Color colour;
	};

	enum Alignment
	{
		None,
		Left,
		Right,
		Center
	};

public:
	UITextBox(Data& data);
	~UITextBox() { }

	void setText(const std::string& text) { mText.setText(text); }

	void setColour(SDL_Color colour) { mText.setColour(colour); }

	int fontSize() const { return mText.fontSize(); }
	void autoSizeFont();

	void autoSizeRectToText();
	VectorF textSize() const { return mText.size(); }

	void align();

	void render() override;

	virtual bool isButton() const { return false; }
	virtual bool hasText() const { return true; }
	virtual Type type() const { return Type::TextBox; }


private:
	Text mText;
	Alignment mAlignment;
};
