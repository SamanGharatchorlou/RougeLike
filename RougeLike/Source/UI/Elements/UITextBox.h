#pragma once

#include "UIBox.h"
#include "Text/Text.h"


//struct UITextBoxData
//{
//	BasicString alignment = "";
//	BasicString font = "";
//	int ptSize = 0;
//	SDL_Color colour;
//	Texture* texture = nullptr;
//	RectF rect;
//	BasicString text;
//};



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

	struct Data : public UIBox::Data
	{
		BasicString alignment = "Center";
		BasicString text = "";
		BasicString font = "";
		int ptSize = 0;
		SDL_Color colour{ 0, 0, 0 };
	};

public:
	UITextBox(Data& data);
	UITextBox(const StringMap& attributes);
	virtual ~UITextBox() { }

	void setText(const BasicString& text) { mText.setText(text); }

	void setColour(SDL_Color colour) { mText.setColour(colour); }

	int fontSize() const { return mText.fontSize(); }
	void autoSizeFont();
	void autoSizeWrapFont();

	void autoSizeRectToText();
	VectorF textSize() const { return mText.size(); }

	void align();

	void render() override;

	virtual Type type() const { return Type::TextBox; }


private:
	void initText(const StringMap& attributes);
	void setAlignment(const BasicString& alignment);


private:
	Text mText;
	Alignment mAlignment;
};
