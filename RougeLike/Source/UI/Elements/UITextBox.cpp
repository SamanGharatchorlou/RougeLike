#include "pch.h"
#include "UITextBox.h"


// UI Text Box
UITextBox::UITextBox(Data& data) : UIBox(data)
{
	if (data.aligment == "Center")
		mAlignment = Alignment::Center;
	else if (data.aligment == "Left")
		mAlignment = Alignment::Left;
	else if (data.aligment == "Right")
		mAlignment = Alignment::Right;
	else
		mAlignment = Alignment::None;

	const std::string& font = data.font == "" ? "default.ttf" : data.font;
	mText.init(data.text, font, data.ptSize, data.colour);

	align();
}

void UITextBox::render()
{
	mText.render(mRect.TopLeft());
	UIBox::render();
}


void UITextBox::autoSizeFont()
{
	VectorF newSize = mText.autoSize(mRect.Size());
}

void UITextBox::align()
{
	if (mAlignment == Alignment::Center)
	{
		float textWidth = mText.size().x;
		float rectWidth = mRect.Width();

		float xOffset = (rectWidth - textWidth) / 2;
		mText.setOffset(VectorF(xOffset, 0.0f));
	}
}