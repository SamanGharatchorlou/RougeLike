#pragma once

#include "UIBox.h"
#include "Text/Text.h"


struct UITextBoxData
{
	BasicString alignment = "";
	BasicString font = "";
	int ptSize = 0;
	SDL_Color colour;
	Texture* texture = nullptr;
	RectF rect;
	BasicString text;
};



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
	UITextBox(const StringMap& attributes);
	UITextBox(const UITextBoxData data);
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
	void initText(const StringMap& attributes);
	void setAlignment(const BasicString& alignment);


private:
	Text mText;
	Alignment mAlignment;
};
