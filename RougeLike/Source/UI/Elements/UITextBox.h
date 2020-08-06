#pragma once

#include "UIBox.h"
#include "Text/Text.h"


class UITextBox : public UIBox
{
public:
	enum Alignment
	{
		None,
		Left,
		Right,
		Center
	};

public:
	UITextBox(const Attributes& attributes);
	virtual ~UITextBox() { }

	void setText(const BasicString& text) { mText.setText(text); }

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
	void initText(const Attributes& attributes);
	void setAlignment(const BasicString& alignment);


private:
	Text mText;
	Alignment mAlignment;
};
